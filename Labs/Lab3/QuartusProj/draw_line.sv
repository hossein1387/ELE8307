module setPixel
    (
        output logic [18:0] addr  ,
        output logic [31:0] data  ,
        output logic        wr    ,
        input  logic        busy  ,
  
        input  logic        clk   ,  
        input  logic        reset ,
        input  logic        clk_en,
        input  logic        start ,
        output logic        done  ,
        input  logic [31:0] dataa ,
        input  logic [31:0] datab ,
        input  logic  [7:0] n     ,
        output logic [31:0] result
    );

    typedef enum logic[5:0] {FETCH_DATA, WRITE_TO_VGA, CALCUL_LINE, CALCUL_INIT, CALC_TRANS1, CALC_TRANS2, CALC_TRANS3, CALC_NEXT_COOR, FINISH} etat_t;
    etat_t etat;

//============================================================================
//    State Machine to extract data and write to VGA controller
//============================================================================
    logic [31:0] x0_temp;
    logic [31:0] y0_temp;
    logic [31:0] x1_temp;
    logic [31:0] y1_temp;
    logic [31:0] color;
    
    logic signed [31:0] xDraw;
    logic signed [31:0] yDraw;
    logic signed [31:0] x0;
    logic signed [31:0] y0;
    logic signed [31:0] x1;
    logic signed [31:0] y1;
    logic signed [31:0] Dx;
    logic signed [31:0] Dy;
    logic signed [31:0] steep;
    logic signed [31:0] ystep;
    logic signed [31:0] xstep;
    logic signed [31:0] TwoDy;
    logic signed [31:0] TwoDx;
    logic signed [31:0] TwoDyTwoDx;
    logic signed [31:0] DiffE;
    logic signed [31:0] x;
    logic signed [31:0] y;
    logic signed [31:0] abs_Dx;
    logic signed [31:0] abs_Dy;

    always @(posedge clk) begin
        if(reset) begin
            wr         <= 1'b0;
            etat       <= FETCH_DATA;
            done       <= 1'b0;
            addr       <= {19{1'b0}};
            result     <= {32{1'b0}};
            data       <= {32{1'b0}};
            xDraw      <= 0;
            yDraw      <= 0;
            TwoDy      <= 0;
            TwoDx      <= 0;
            TwoDyTwoDx <= 0;
            DiffE      <= 0;
            x          <= 0;
            y          <= 0;
            x0         <= 0;
            y0         <= 0;
            x1         <= 0;
            y1         <= 0;
            Dx         <= 0;
            Dy         <= 0;    
            steep      <= 0;
            ystep      <= 0;
            xstep      <= 0;
            abs_Dx     <= 0;
            abs_Dy     <= 0;

            end else begin
            if(clk_en) begin
                case(etat)
                    FETCH_DATA:
                        begin
                            if( start == 1'b1 ) begin
                                x0_temp   <= 32'(dataa[9:0]);
                                y0_temp   <= 32'(dataa[19:10]);
                                x1_temp   <= 32'(dataa[29:20]);
                                y1_temp   <= 32'(datab[9:0]);
                                color     <= 32'(datab[19:10]);
                                x0        <= 32'(dataa[9:0]);
                                y0        <= 32'(dataa[19:10]);
                                x1        <= 32'(dataa[29:20]);
                                y1        <= 32'(datab[9:0]);
                                Dx        <= dataa[29:20] - dataa[9:0];
                                Dy        <= datab[9:0]   - dataa[19:10];
                                etat      <= (n==0) ? WRITE_TO_VGA : CALC_TRANS1;
                            end else begin
                                etat   <=  FETCH_DATA;
                            end;
                            done   <= 1'b0;
                            wr     <= 1'b0;
                        end
                    CALC_TRANS1:
                        begin
                            abs_Dy <= (signed'(Dy)<0) ? -Dy : Dy;
                            abs_Dx <= (signed'(Dx)<0) ? -Dx : Dx;
                            etat <= CALC_TRANS2;
                       end
                    CALC_TRANS2:
                        begin
                            steep  <= (abs_Dy >= abs_Dx) ? 1:0;
                            if (abs_Dy >= abs_Dx) begin
                                x0 <= y0_temp;
                                x1 <= y1_temp;
                                y0 <= x0_temp;
                                y1 <= x1_temp;
                                Dx <= (y1_temp - y0_temp);
                                Dy <= (x1_temp - x0_temp);
                            end
                            etat <= CALC_TRANS3;
                        end                     
                    CALC_TRANS3:
                        begin
                            if((signed'(Dx)<0)) begin
                                xstep <= -1;
                                Dx    <= -Dx;
                            end else begin
                                xstep <= 1;
                            end
                            if((signed'(Dy))<0) begin
                                ystep <= -1;
                                Dy    <= -Dy;
                            end else begin
                                ystep <= 1;
                            end
                            etat <= CALCUL_INIT;
                        end
                    CALCUL_INIT:
                        //TwoDy      <= Dy<<1;
                        //TwoDx      <= Dx<<1;
                        begin
                            TwoDyTwoDx <= (Dy<<1) - (Dx<<1);
                            DiffE      <= (Dy<<1) - Dx;
                            y          <= y0;
                            x          <= x0;
                            etat       <= CALCUL_LINE;
                        end
                    CALCUL_LINE:
                        begin
                            if(x != x1) begin
                                if(steep) begin
                                    xDraw <= y;
                                    yDraw <= x;
                                end else begin
                                    xDraw <= x;
                                    yDraw <= y;
                                end
                                etat <= WRITE_TO_VGA;
                            end else begin
                                etat <= FINISH;                            
                            end
                        end 
                    WRITE_TO_VGA:
                        begin
                            if( busy == 1'b0 ) begin 
                                if(n==0) begin
                                    done   <= 1'b1;
                                    wr     <= 1'b1;
                                    result <= datab;
                                    addr   <= 19'(x0_temp + (y0_temp<<9) + (y0_temp<<7)) ;
                                    data   <= 32'(color);
                                    etat   <= FETCH_DATA  ;
                                end else begin
                                    wr     <= 1'b1;
                                    result <= datab;
                                    addr   <= 19'(xDraw + (yDraw<<9) + (yDraw<<7)) ;
                                    data   <= 32'(color);
                                    etat   <= CALC_NEXT_COOR;
                                end
                            end else begin
                                etat   <=  WRITE_TO_VGA ;
                            end
                        end
                    CALC_NEXT_COOR:
                        begin
                            wr     <= 1'b0;
                            if(signed'(DiffE)>0) begin
                                DiffE <= DiffE + TwoDyTwoDx;
                                y     <= y + ystep;
                            end else begin
                                DiffE <= DiffE + (Dy<<1);
                            end
                            x      <= x + xstep;
                            etat   <= CALCUL_LINE;
                        end
                    FINISH:
                        begin
                            done   <= 1'b1;
                            etat   <= FETCH_DATA;
                        end
                endcase
            end
        end
    end

//============================================================================
// circuit to calculate line coefficients
//============================================================================

    

endmodule // setPixel
