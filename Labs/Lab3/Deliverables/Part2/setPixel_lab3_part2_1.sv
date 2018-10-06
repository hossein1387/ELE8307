module setPixel
    (
        input  logic signed [31:0] dataa, //x
        input  logic signed [31:0] datab, //y
        output logic signed [31:0] result
    );
    logic signed [31:0] BASE_ADDR = 32'h0100_0000;
    assign result = (BASE_ADDR>>2) + dataa + (datab<<7) + (datab<<9);
endmodule // setPixel

