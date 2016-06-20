`timescale 1ns / 1ps

module jit_blackbox(
  output wire             sI1_V_TREADY  ,
  input  wire             sI1_V_TVALID  ,
  input  wire  [31 : 0]   sI1_V_TDATA   ,

  output wire             sI2_V_TREADY  ,
  input  wire             sI2_V_TVALID  ,
  input  wire  [31 : 0]   sI2_V_TDATA   ,

  input  wire             mO1_V_TREADY  ,
  output wire             mO1_V_TVALID  ,
  output wire  [31 : 0]   mO1_V_TDATA   ,

  input  wire  [15 : 0]   arg1_V        ,
  input  wire  [15 : 0]   arg2_V        ,
  input  wire  [15 : 0]   arg3_V        ,

  input  wire             ap_clk        ,
  input  wire             ap_rst_n
);

  wire [7:0]  wpip;
  wire [3:0]  wtype;
  wire [7:4]  wout;
  wire [31:0] wcal;

  reg [31:0] rj;
  reg [31:0] ri;
  reg [31:0] rk;

  wire pip_condition   ;
  wire cal_condition   ;
  wire out_condition   ;
  wire start_condition ;

  assign wpip  = arg3_V[15:8];
  assign wout  = arg3_V[ 7:4];
  assign wtype = arg3_V[ 3:0];

  assign sI1_V_TREADY = wtype == 1 ? sI1_V_TVALID && (state == CAL) : start_condition && (state == CAL);
  assign sI2_V_TREADY = wtype == 1 ? 1'b0 : start_condition && (state == CAL);


  assign mO1_V_TVALID = wtype == 1 ? (state == CAL && sI1_V_TVALID && mO1_V_TREADY) : ((state == CAL && sI1_V_TVALID && sI2_V_TVALID && mO1_V_TREADY) || (state == OUT && mO1_V_TREADY));
  assign mO1_V_TDATA  = wtype == 1 ? sI1_V_TDATA : sI1_V_TDATA + sI2_V_TDATA;

  assign wcal  = {{arg1_V}, {arg2_V}};

  assign pip_condition   = rj < wpip - 2 ? 0 : 1;
  assign cal_condition   = ri < wcal - 1 ? 0 : 1;
  assign out_condition   = rk < wcal - 1 ? 0 : 1;

  assign start_condition = wtype == 1 ? (sI1_V_TVALID == 1) : (sI1_V_TVALID == 1 && sI2_V_TVALID == 1);

  always @(posedge ap_clk) begin
    if (!ap_rst_n || cal_condition) begin
      ri <= 32'd0;
    end
    else begin
      if (start_condition && state == CAL) begin
        ri <= ri + 1;
      end
      else begin
        ri <= ri;
      end
    end
  end

  always @(posedge ap_clk) begin
    if (!ap_rst_n | pip_condition) begin
      rj <= 32'd0;
    end
    else begin
      if (state == PIP) begin
        rj <= rj + 1;
      end
      else begin
        rj <= rj;
      end
    end
  end

  always @(posedge ap_clk) begin
    if (!ap_rst_n || out_condition) begin
      rk <= 32'd0;
    end
    else begin
      if (state == OUT) begin
        rk <= rk + 1;
      end
      else begin
        rk <= rk;
      end
    end
  end

  localparam IDEL = 4'b0001;
  localparam PIP  = 4'b0010;
  localparam CAL  = 4'b0100;
  localparam OUT  = 4'b1000;

  reg [ 4 : 0] state, next;

  always @(posedge ap_clk) begin
    if (!ap_rst_n) begin
      state <= IDEL;
    end
    else begin
      state <= next;
    end
  end

  always @(*) begin
    next <= IDEL;

    case(state)
      IDEL : begin
        if (start_condition) begin
          if (wpip > 0) begin
            next <= PIP;
          end
          else begin
            next <= CAL;
          end
        end
        else begin
          next <= IDEL;
        end
      end

      PIP  : begin
        if (pip_condition) begin
          next <= CAL;
        end
        else begin
          next <= PIP;
        end
      end

      CAL  : begin
        if (cal_condition) begin
          if (wout) begin
            next <= OUT;
          end
          else begin
            next <= IDEL;
          end
        end
        else begin
          next <= CAL;
        end
      end

      OUT  : begin
        if (out_condition) begin
          next <= IDEL;
        end
        else begin
          next <= OUT;
        end
      end
    endcase
  end

endmodule
////////////////////////////////////////////////////////////////////////////////////////////////////
  // wire [7:0]  wpip;
  // wire [3:0]  wtype;
  // wire [7:4]  wout;
  // wire [31:0] wcal;

  // reg [31:0] rj;
  // reg [31:0] ri;
  // reg [31:0] rk;

  // wire pip_condition   ;
  // wire cal_condition   ;
  // wire out_condition   ;
  // wire dule_valide ;

  // assign wpip  = arg3_V[15:8];
  // assign wout  = arg3_V[ 7:4];
  // assign wtype = arg3_V[ 3:0];
  // assign wcal  = {{arg1_V}, {arg2_V}};

  // assign pip_condition   = rj < wpip - 2 ? 0 : 1;
  // assign cal_condition   = ri < wcal - 1 ? 0 : 1;
  // assign out_condition   = rk < wcal - 1 ? 0 : 1;

  // assign dule_valide = sI1_V_TVALID && sI2_V_TVALID;

  // assign sI1_V_TREADY = wtype == 1 ? sI1_V_TVALID : dule_valide;
  // assign sI2_V_TREADY = wtype == 1 ? 1'b0 : dule_valide;
  // assign mO1_V_TVALID = wtype == 1 ? (state == CAL && sI1_V_TVALID && mO1_V_TREADY) : ((state == CAL && sI1_V_TVALID && sI2_V_TVALID && mO1_V_TREADY) || (state == OUT && mO1_V_TREADY));
  // assign mO1_V_TDATA  = wtype == 1 ? sI1_V_TDATA : sI1_V_TDATA + sI2_V_TDATA;

  // always @(posedge ap_clk) begin
  //   if (!ap_rst_n || cal_condition) begin
  //     ri <= 32'd0;
  //   end
  //   else begin
  //     if (dule_valide && state == CAL) begin
  //       ri <= ri + 1;
  //     end
  //     else begin
  //       ri <= ri;
  //     end
  //   end
  // end

  // always @(posedge ap_clk) begin
  //   if (!ap_rst_n | pip_condition) begin
  //     rj <= 32'd0;
  //   end
  //   else begin
  //     if (state == PIP) begin
  //       rj <= rj + 1;
  //     end
  //     else begin
  //       rj <= rj;
  //     end
  //   end
  // end

  // always @(posedge ap_clk) begin
  //   if (!ap_rst_n || out_condition) begin
  //     rk <= 32'd0;
  //   end
  //   else begin
  //     if (state == OUT) begin
  //       rk <= rk + 1;
  //     end
  //     else begin
  //       rk <= rk;
  //     end
  //   end
  // end

  // localparam IDLE = 4'b0001;
  // localparam PIP  = 4'b0010;
  // localparam CAL  = 4'b0100;
  // localparam OUT  = 4'b1000;

  // reg [ 4 : 0] state, next;

  // always @(posedge ap_clk) begin
  //   if (!ap_rst_n) begin
  //     state <= IDLE;
  //   end
  //   else begin
  //     state <= next;
  //   end
  // end

  // always @(*) begin
  //   next <= IDLE;

  //   case(state)
  //     IDLE : begin
  //       if (dule_valide) begin
  //         if (wpip > 0) begin
  //           next <= PIP;
  //         end
  //         else begin
  //           next <= CAL;
  //         end
  //       end
  //       else begin
  //         next <= IDLE;
  //       end
  //     end

  //     PIP  : begin
  //       if (pip_condition) begin
  //         next <= CAL;
  //       end
  //       else begin
  //         next <= PIP;
  //       end
  //     end

  //     CAL  : begin
  //       if (cal_condition) begin
  //         if (wout) begin
  //           next <= OUT;
  //         end
  //         else begin
  //           next <= IDLE;
  //         end
  //       end
  //       else begin
  //         next <= CAL;
  //       end
  //     end

  //     OUT  : begin
  //       if (out_condition) begin
  //         next <= IDLE;
  //       end
  //       else begin
  //         next <= OUT;
  //       end
  //     end
  //   endcase
  // end



////////////////////////////////////////////////////////////////////////////////////////////////////


  // assign sI1_V_TREADY = (state == CAL);
  // assign sI2_V_TREADY = wtype == 1 ? 1'b0 : (state == CAL);
  // assign start_condition = wtype == 1 ? (sI1_V_TVALID == 1) : (sI1_V_TVALID == 1 && sI2_V_TVALID == 1);





  // localparam IDLE = 4'b0001;
  // localparam PIP  = 4'b0010;
  // localparam CAL  = 4'b0100;
  // localparam OUT  = 4'b1000;

  // reg [ 4 : 0] state, next;



  // always @(*) begin
  //   next <= IDLE;

  //   case(state)
  //     IDLE : begin
  //       if (start_condition) begin
  //         if (wpip > 0) begin
  //           next <= PIP;
  //         end
  //         else begin
  //           next <= CAL;
  //         end
  //       end
  //       else begin
  //         next <= IDLE;
  //       end
  //     end

  //     PIP  : begin
  //       if (pip_condition) begin
  //         next <= CAL;
  //       end
  //       else begin
  //         next <= PIP;
  //       end
  //     end

  //     CAL  : begin
  //       if (cal_condition) begin
  //         if (wout) begin
  //           next <= OUT;
  //         end
  //         else begin
  //           next <= IDLE;
  //         end
  //       end
  //       else begin
  //         next <= CAL;
  //       end
  //     end

  //     OUT  : begin
  //       if (out_condition) begin
  //         next <= IDLE;
  //       end
  //       else begin
  //         next <= OUT;
  //       end
  //     end
  //   endcase
  // end


