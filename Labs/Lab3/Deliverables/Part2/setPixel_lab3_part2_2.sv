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
        output logic [31:0] result
    );

    typedef enum logic[5:0] {FETCH_DATA, WAIT_FOR_BUSY, WRITE_DATA} etat_t;
    etat_t etat;

    logic signed[15:0] x;
    logic signed[15:0] y;
    logic       [31:0] color;

    always @(posedge clk) begin
        if(reset) begin
             wr     <= 1'b0;
             etat   <= FETCH_DATA;
             done   <= 1'b0;
             addr   <= {19{1'b0}};
             result <= {32{1'b0}};
             data   <= {32{1'b0}};
        end else begin
            if(clk_en) begin
                case(etat)
                    FETCH_DATA:
                        begin
                            if( start == 1'b1 ) begin
                                x      <= dataa[15:0];
                                y      <= dataa[31:16];
                                color  <= datab;
                                etat   <= WAIT_FOR_BUSY;
                            end else begin
                                etat   <=  FETCH_DATA;
                            end;
                            done   <= 1'b0;
                            wr     <= 1'b0;
                        end
                    WAIT_FOR_BUSY:
                        begin
                            if( busy == 1'b0 ) begin 
                                done   <= 1'b1;
                                wr     <= 1'b1;
                                result <= dataa; // debug feature!
                                addr   <= x + (y<<9) + (y<<7) ;
                                data   <= color;                                
                                etat   <= FETCH_DATA;    
                            end else begin
                                etat <= WAIT_FOR_BUSY;
                            end
                        end
                    //WRITE_DATA:
                    //    begin
                    //    end
                endcase
            end
        end
    end
endmodule // setPixel
