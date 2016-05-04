module jit #(
  parameter integer NUM_ACCs = 8
)
(
  output  wire            s11i_rdy     ,
  input   wire            s11i_valid   ,
  input   wire  [31 : 0]  s11i_data    ,
  output  wire            s12i_rdy     ,
  input   wire            s12i_valid   ,
  input   wire  [31 : 0]  s12i_data    ,
  input   wire            s13o_rdy     ,
  output  wire            s13o_valid   ,
  output  wire  [31 : 0]  s13o_data    ,
  //----------------------------------//
  output  wire            s14i_rdy     ,
  input   wire            s14i_valid   ,
  input   wire  [31 : 0]  s14i_data    ,
  output  wire            s15i_rdy     ,
  input   wire            s15i_valid   ,
  input   wire  [31 : 0]  s15i_data    ,
  input   wire            s16o_rdy     ,
  output  wire            s16o_valid   ,
  output  wire  [31 : 0]  s16o_data    ,
  //////////////////////////////////////
  output  wire            s21i_rdy     ,
  input   wire            s21i_valid   ,
  input   wire  [31 : 0]  s21i_data    ,
  output  wire            s22i_rdy     ,
  input   wire            s22i_valid   ,
  input   wire  [31 : 0]  s22i_data    ,
  input   wire            s23o_rdy     ,
  output  wire            s23o_valid   ,
  output  wire  [31 : 0]  s23o_data    ,
  //----------------------------------//
  output  wire            s24i_rdy     ,
  input   wire            s24i_valid   ,
  input   wire  [31 : 0]  s24i_data    ,
  output  wire            s25i_rdy     ,
  input   wire            s25i_valid   ,
  input   wire  [31 : 0]  s25i_data    ,
  input   wire            s26o_rdy     ,
  output  wire            s26o_valid   ,
  output  wire  [31 : 0]  s26o_data    ,
  //////////////////////////////////////
  output  wire            s31i_rdy     ,
  input   wire            s31i_valid   ,
  input   wire  [31 : 0]  s31i_data    ,
  output  wire            s32i_rdy     ,
  input   wire            s32i_valid   ,
  input   wire  [31 : 0]  s32i_data    ,
  input   wire            s33o_rdy     ,
  output  wire            s33o_valid   ,
  output  wire  [31 : 0]  s33o_data    ,
  //----------------------------------//
  output  wire            s34i_rdy     ,
  input   wire            s34i_valid   ,
  input   wire  [31 : 0]  s34i_data    ,
  output  wire            s35i_rdy     ,
  input   wire            s35i_valid   ,
  input   wire  [31 : 0]  s35i_data    ,
  input   wire            s36o_rdy     ,
  output  wire            s36o_valid   ,
  output  wire  [31 : 0]  s36o_data    ,
  //////////////////////////////////////
  output  wire            s41i_rdy     ,
  input   wire            s41i_valid   ,
  input   wire  [31 : 0]  s41i_data    ,
  output  wire            s42i_rdy     ,
  input   wire            s42i_valid   ,
  input   wire  [31 : 0]  s42i_data    ,
  input   wire            s43o_rdy     ,
  output  wire            s43o_valid   ,
  output  wire  [31 : 0]  s43o_data    ,
  //----------------------------------//
  output  wire            s44i_rdy     ,
  input   wire            s44i_valid   ,
  input   wire  [31 : 0]  s44i_data    ,
  output  wire            s45i_rdy     ,
  input   wire            s45i_valid   ,
  input   wire  [31 : 0]  s45i_data    ,
  input   wire            s46o_rdy     ,
  output  wire            s46o_valid   ,
  output  wire  [31 : 0]  s46o_data    ,
  //////////////////////////////////////
  output  wire            s51i_rdy     ,
  input   wire            s51i_valid   ,
  input   wire  [31 : 0]  s51i_data    ,
  output  wire            s52i_rdy     ,
  input   wire            s52i_valid   ,
  input   wire  [31 : 0]  s52i_data    ,
  input   wire            s53o_rdy     ,
  output  wire            s53o_valid   ,
  output  wire  [31 : 0]  s53o_data    ,
  //----------------------------------//
  output  wire            s54i_rdy     ,
  input   wire            s54i_valid   ,
  input   wire  [31 : 0]  s54i_data    ,
  output  wire            s55i_rdy     ,
  input   wire            s55i_valid   ,
  input   wire  [31 : 0]  s55i_data    ,
  input   wire            s56o_rdy     ,
  output  wire            s56o_valid   ,
  output  wire  [31 : 0]  s56o_data    ,
  //////////////////////////////////////
  output  wire            s61i_rdy     ,
  input   wire            s61i_valid   ,
  input   wire  [31 : 0]  s61i_data    ,
  output  wire            s62i_rdy     ,
  input   wire            s62i_valid   ,
  input   wire  [31 : 0]  s62i_data    ,
  input   wire            s63o_rdy     ,
  output  wire            s63o_valid   ,
  output  wire  [31 : 0]  s63o_data    ,
  //----------------------------------//
  output  wire            s64i_rdy     ,
  input   wire            s64i_valid   ,
  input   wire  [31 : 0]  s64i_data    ,
  output  wire            s65i_rdy     ,
  input   wire            s65i_valid   ,
  input   wire  [31 : 0]  s65i_data    ,
  input   wire            s66o_rdy     ,
  output  wire            s66o_valid   ,
  output  wire  [31 : 0]  s66o_data    ,
  //////////////////////////////////////
  output  wire            s71i_rdy     ,
  input   wire            s71i_valid   ,
  input   wire  [31 : 0]  s71i_data    ,
  output  wire            s72i_rdy     ,
  input   wire            s72i_valid   ,
  input   wire  [31 : 0]  s72i_data    ,
  input   wire            s73o_rdy     ,
  output  wire            s73o_valid   ,
  output  wire  [31 : 0]  s73o_data    ,
  //----------------------------------//
  output  wire            s74i_rdy     ,
  input   wire            s74i_valid   ,
  input   wire  [31 : 0]  s74i_data    ,
  output  wire            s75i_rdy     ,
  input   wire            s75i_valid   ,
  input   wire  [31 : 0]  s75i_data    ,
  input   wire            s76o_rdy     ,
  output  wire            s76o_valid   ,
  output  wire  [31 : 0]  s76o_data    ,
  //////////////////////////////////////
  output  wire            s81i_rdy     ,
  input   wire            s81i_valid   ,
  input   wire  [31 : 0]  s81i_data    ,
  output  wire            s82i_rdy     ,
  input   wire            s82i_valid   ,
  input   wire  [31 : 0]  s82i_data    ,
  input   wire            s83o_rdy     ,
  output  wire            s83o_valid   ,
  output  wire  [31 : 0]  s83o_data    ,
  //----------------------------------//
  output  wire            s84i_rdy     ,
  input   wire            s84i_valid   ,
  input   wire  [31 : 0]  s84i_data    ,
  output  wire            s85i_rdy     ,
  input   wire            s85i_valid   ,
  input   wire  [31 : 0]  s85i_data    ,
  input   wire            s86o_rdy     ,
  output  wire            s86o_valid   ,
  output  wire  [31 : 0]  s86o_data    ,
  //////////////////////////////////////
  output  wire            s50i_rdy     ,
  input   wire            s50i_valid   ,
  input   wire  [31 : 0]  s50i_data    ,
//input   wire            s50o_rdy     ,
//output  wire            s50o_valid   ,
//output  wire  [31 : 0]  s50o_data    ,
////////////////////////////////////////
  output  wire            s100i_rdy    ,
  input   wire            s100i_valid  ,
  input   wire  [31 : 0]  s100i_data   ,
  //////////////////////////////////////
  input   wire            clk          ,
  input   wire            rst
);
          wire            clk_100       ;
          wire            rstn          ;
          wire            clk_locked    ;
          wire            icap_output   ;
          wire  [31 : 0]  swapped_idata ;

          wire            ws11i_rdy     ;
          wire            ws11i_valid   ;
          wire  [31 : 0]  ws11i_data    ;
          wire            ws12i_rdy     ;
          wire            ws12i_valid   ;
          wire  [31 : 0]  ws12i_data    ;
          wire            ws13o_rdy     ;
          wire            ws13o_valid   ;
          wire  [31 : 0]  ws13o_data    ;
          //---------------------------//
          wire            ws14i_rdy     ;
          wire            ws14i_valid   ;
          wire  [31 : 0]  ws14i_data    ;
          wire            ws15i_rdy     ;
          wire            ws15i_valid   ;
          wire  [31 : 0]  ws15i_data    ;
          wire            ws16o_rdy     ;
          wire            ws16o_valid   ;
          wire  [31 : 0]  ws16o_data    ;
          ///////////////////////////////
          wire            ws21i_rdy     ;
          wire            ws21i_valid   ;
          wire  [31 : 0]  ws21i_data    ;
          wire            ws22i_rdy     ;
          wire            ws22i_valid   ;
          wire  [31 : 0]  ws22i_data    ;
          wire            ws23o_rdy     ;
          wire            ws23o_valid   ;
          wire  [31 : 0]  ws23o_data    ;
          //---------------------------//
          wire            ws24i_rdy     ;
          wire            ws24i_valid   ;
          wire  [31 : 0]  ws24i_data    ;
          wire            ws25i_rdy     ;
          wire            ws25i_valid   ;
          wire  [31 : 0]  ws25i_data    ;
          wire            ws26o_rdy     ;
          wire            ws26o_valid   ;
          wire  [31 : 0]  ws26o_data    ;
          ///////////////////////////////
          wire            ws31i_rdy     ;
          wire            ws31i_valid   ;
          wire  [31 : 0]  ws31i_data    ;
          wire            ws32i_rdy     ;
          wire            ws32i_valid   ;
          wire  [31 : 0]  ws32i_data    ;
          wire            ws33o_rdy     ;
          wire            ws33o_valid   ;
          wire  [31 : 0]  ws33o_data    ;
          //---------------------------//
          wire            ws34i_rdy     ;
          wire            ws34i_valid   ;
          wire  [31 : 0]  ws34i_data    ;
          wire            ws35i_rdy     ;
          wire            ws35i_valid   ;
          wire  [31 : 0]  ws35i_data    ;
          wire            ws36o_rdy     ;
          wire            ws36o_valid   ;
          wire  [31 : 0]  ws36o_data    ;
          ///////////////////////////////
          wire            ws41i_rdy     ;
          wire            ws41i_valid   ;
          wire  [31 : 0]  ws41i_data    ;
          wire            ws42i_rdy     ;
          wire            ws42i_valid   ;
          wire  [31 : 0]  ws42i_data    ;
          wire            ws43o_rdy     ;
          wire            ws43o_valid   ;
          wire  [31 : 0]  ws43o_data    ;
          //---------------------------//
          wire            ws44i_rdy     ;
          wire            ws44i_valid   ;
          wire  [31 : 0]  ws44i_data    ;
          wire            ws45i_rdy     ;
          wire            ws45i_valid   ;
          wire  [31 : 0]  ws45i_data    ;
          wire            ws46o_rdy     ;
          wire            ws46o_valid   ;
          wire  [31 : 0]  ws46o_data    ;
          ///////////////////////////////
          wire            ws51i_rdy     ;
          wire            ws51i_valid   ;
          wire  [31 : 0]  ws51i_data    ;
          wire            ws52i_rdy     ;
          wire            ws52i_valid   ;
          wire  [31 : 0]  ws52i_data    ;
          wire            ws53o_rdy     ;
          wire            ws53o_valid   ;
          wire  [31 : 0]  ws53o_data    ;
          //---------------------------//
          wire            ws54i_rdy     ;
          wire            ws54i_valid   ;
          wire  [31 : 0]  ws54i_data    ;
          wire            ws55i_rdy     ;
          wire            ws55i_valid   ;
          wire  [31 : 0]  ws55i_data    ;
          wire            ws56o_rdy     ;
          wire            ws56o_valid   ;
          wire  [31 : 0]  ws56o_data    ;
          ///////////////////////////////
          wire            ws61i_rdy     ;
          wire            ws61i_valid   ;
          wire  [31 : 0]  ws61i_data    ;
          wire            ws62i_rdy     ;
          wire            ws62i_valid   ;
          wire  [31 : 0]  ws62i_data    ;
          wire            ws63o_rdy     ;
          wire            ws63o_valid   ;
          wire  [31 : 0]  ws63o_data    ;
          //---------------------------//
          wire            ws64i_rdy     ;
          wire            ws64i_valid   ;
          wire  [31 : 0]  ws64i_data    ;
          wire            ws65i_rdy     ;
          wire            ws65i_valid   ;
          wire  [31 : 0]  ws65i_data    ;
          wire            ws66o_rdy     ;
          wire            ws66o_valid   ;
          wire  [31 : 0]  ws66o_data    ;
          ///////////////////////////////
          wire            ws71i_rdy     ;
          wire            ws71i_valid   ;
          wire  [31 : 0]  ws71i_data    ;
          wire            ws72i_rdy     ;
          wire            ws72i_valid   ;
          wire  [31 : 0]  ws72i_data    ;
          wire            ws73o_rdy     ;
          wire            ws73o_valid   ;
          wire  [31 : 0]  ws73o_data    ;
          //---------------------------//
          wire            ws74i_rdy     ;
          wire            ws74i_valid   ;
          wire  [31 : 0]  ws74i_data    ;
          wire            ws75i_rdy     ;
          wire            ws75i_valid   ;
          wire  [31 : 0]  ws75i_data    ;
          wire            ws76o_rdy     ;
          wire            ws76o_valid   ;
          wire  [31 : 0]  ws76o_data    ;
          ///////////////////////////////
          wire            ws81i_rdy     ;
          wire            ws81i_valid   ;
          wire  [31 : 0]  ws81i_data    ;
          wire            ws82i_rdy     ;
          wire            ws82i_valid   ;
          wire  [31 : 0]  ws82i_data    ;
          wire            ws83o_rdy     ;
          wire            ws83o_valid   ;
          wire  [31 : 0]  ws83o_data    ;
          //---------------------------//
          wire            ws84i_rdy     ;
          wire            ws84i_valid   ;
          wire  [31 : 0]  ws84i_data    ;
          wire            ws85i_rdy     ;
          wire            ws85i_valid   ;
          wire  [31 : 0]  ws85i_data    ;
          wire            ws86o_rdy     ;
          wire            ws86o_valid   ;
          wire  [31 : 0]  ws86o_data    ;
          ///////////////////////////////
          wire            ws50i_rdy     ;
          wire            ws50i_valid   ;
          wire  [31 : 0]  ws50i_data    ;
          wire            ws50o_rdy     ;
          wire            ws50o_valid   ;
          wire  [31 : 0]  ws50o_data    ;
          ///////////////////////////////
          wire            ws100i_rdy    ;
          wire            ws100i_valid  ;
          wire  [31 : 0]  ws100i_data   ;
          ///////////////////////////////
          wire            wacc1C_tready ;
          wire            wacc1C_tvalid ;
          wire  [31 : 0]  wacc1C_tdata  ;
          wire            wacc1A_tready ;
          wire            wacc1A_tvalid ;
          wire  [31 : 0]  wacc1A_tdata  ;
          wire            wacc1B_tready ;
          wire            wacc1B_tvalid ;
          wire  [31 : 0]  wacc1B_tdata  ;
          wire            wacc1P_start  ;
          wire            wacc1P_done   ;
          wire            wacc1P_idle   ;
          wire            wacc1P_ready  ;
          wire  [15 : 0]  wacc1P_arg1_V ;
          wire  [15 : 0]  wacc1P_arg2_V ;
          wire  [15 : 0]  wacc1P_arg3_V ;
          wire            wffo1C_tready ;
          wire            wffo1C_tvalid ;
          wire  [31 : 0]  wffo1C_tdata  ;
          wire            wffo1A_tready ;
          wire            wffo1A_tvalid ;
          wire  [31 : 0]  wffo1A_tdata  ;
          wire            wffo1B_tready ;
          wire            wffo1B_tvalid ;
          wire  [31 : 0]  wffo1B_tdata  ;
          wire             wPR1_DONE    ;
          wire            dffo1A_tready ;
          wire            dffo1B_tready ;
          wire            dffo1C_tvalid ;
          wire  [31 : 0]  dffo1C_tdata  ;
          wire            dacc1P_done   ;

          wire            wacc2C_tready ;
          wire            wacc2C_tvalid ;
          wire  [31 : 0]  wacc2C_tdata  ;
          wire            wacc2A_tready ;
          wire            wacc2A_tvalid ;
          wire  [31 : 0]  wacc2A_tdata  ;
          wire            wacc2B_tready ;
          wire            wacc2B_tvalid ;
          wire  [31 : 0]  wacc2B_tdata  ;
          wire            wacc2P_start  ;
          wire            wacc2P_done   ;
          wire            wacc2P_idle   ;
          wire            wacc2P_ready  ;
          wire  [15 : 0]  wacc2P_arg1_V ;
          wire  [15 : 0]  wacc2P_arg2_V ;
          wire  [15 : 0]  wacc2P_arg3_V ;
          wire            wffo2C_tready ;
          wire            wffo2C_tvalid ;
          wire  [31 : 0]  wffo2C_tdata  ;
          wire            wffo2A_tready ;
          wire            wffo2A_tvalid ;
          wire  [31 : 0]  wffo2A_tdata  ;
          wire            wffo2B_tready ;
          wire            wffo2B_tvalid ;
          wire  [31 : 0]  wffo2B_tdata  ;
          wire             wPR2_DONE    ;
          wire            dffo2A_tready ;
          wire            dffo2B_tready ;
          wire            dffo2C_tvalid ;
          wire  [31 : 0]  dffo2C_tdata  ;
          wire            dacc2P_done   ;

          wire            wacc3C_tready ;
          wire            wacc3C_tvalid ;
          wire  [31 : 0]  wacc3C_tdata  ;
          wire            wacc3A_tready ;
          wire            wacc3A_tvalid ;
          wire  [31 : 0]  wacc3A_tdata  ;
          wire            wacc3B_tready ;
          wire            wacc3B_tvalid ;
          wire  [31 : 0]  wacc3B_tdata  ;
          wire            wacc3P_start  ;
          wire            wacc3P_done   ;
          wire            wacc3P_idle   ;
          wire            wacc3P_ready  ;
          wire  [15 : 0]  wacc3P_arg1_V ;
          wire  [15 : 0]  wacc3P_arg2_V ;
          wire  [15 : 0]  wacc3P_arg3_V ;
          wire            wffo3C_tready ;
          wire            wffo3C_tvalid ;
          wire  [31 : 0]  wffo3C_tdata  ;
          wire            wffo3A_tready ;
          wire            wffo3A_tvalid ;
          wire  [31 : 0]  wffo3A_tdata  ;
          wire            wffo3B_tready ;
          wire            wffo3B_tvalid ;
          wire  [31 : 0]  wffo3B_tdata  ;
          wire             wPR3_DONE    ;
          wire            dffo3A_tready ;
          wire            dffo3B_tready ;
          wire            dffo3C_tvalid ;
          wire  [31 : 0]  dffo3C_tdata  ;
          wire            dacc3P_done   ;

          wire            wacc4C_tready ;
          wire            wacc4C_tvalid ;
          wire  [31 : 0]  wacc4C_tdata  ;
          wire            wacc4A_tready ;
          wire            wacc4A_tvalid ;
          wire  [31 : 0]  wacc4A_tdata  ;
          wire            wacc4B_tready ;
          wire            wacc4B_tvalid ;
          wire  [31 : 0]  wacc4B_tdata  ;
          wire            wacc4P_start  ;
          wire            wacc4P_done   ;
          wire            wacc4P_idle   ;
          wire            wacc4P_ready  ;
          wire  [15 : 0]  wacc4P_arg1_V ;
          wire  [15 : 0]  wacc4P_arg2_V ;
          wire  [15 : 0]  wacc4P_arg3_V ;
          wire            wffo4C_tready ;
          wire            wffo4C_tvalid ;
          wire  [31 : 0]  wffo4C_tdata  ;
          wire            wffo4A_tready ;
          wire            wffo4A_tvalid ;
          wire  [31 : 0]  wffo4A_tdata  ;
          wire            wffo4B_tready ;
          wire            wffo4B_tvalid ;
          wire  [31 : 0]  wffo4B_tdata  ;
          wire             wPR4_DONE    ;
          wire            dffo4A_tready ;
          wire            dffo4B_tready ;
          wire            dffo4C_tvalid ;
          wire  [31 : 0]  dffo4C_tdata  ;
          wire            dacc4P_done   ;

          wire            wacc5C_tready ;
          wire            wacc5C_tvalid ;
          wire  [31 : 0]  wacc5C_tdata  ;
          wire            wacc5A_tready ;
          wire            wacc5A_tvalid ;
          wire  [31 : 0]  wacc5A_tdata  ;
          wire            wacc5B_tready ;
          wire            wacc5B_tvalid ;
          wire  [31 : 0]  wacc5B_tdata  ;
          wire            wacc5P_start  ;
          wire            wacc5P_done   ;
          wire            wacc5P_idle   ;
          wire            wacc5P_ready  ;
          wire  [15 : 0]  wacc5P_arg1_V ;
          wire  [15 : 0]  wacc5P_arg2_V ;
          wire  [15 : 0]  wacc5P_arg3_V ;
          wire            wffo5C_tready ;
          wire            wffo5C_tvalid ;
          wire  [31 : 0]  wffo5C_tdata  ;
          wire            wffo5A_tready ;
          wire            wffo5A_tvalid ;
          wire  [31 : 0]  wffo5A_tdata  ;
          wire            wffo5B_tready ;
          wire            wffo5B_tvalid ;
          wire  [31 : 0]  wffo5B_tdata  ;
          wire             wPR5_DONE    ;
          wire            dffo5A_tready ;
          wire            dffo5B_tready ;
          wire            dffo5C_tvalid ;
          wire  [31 : 0]  dffo5C_tdata  ;
          wire            dacc5P_done   ;

          wire            wacc6C_tready ;
          wire            wacc6C_tvalid ;
          wire  [31 : 0]  wacc6C_tdata  ;
          wire            wacc6A_tready ;
          wire            wacc6A_tvalid ;
          wire  [31 : 0]  wacc6A_tdata  ;
          wire            wacc6B_tready ;
          wire            wacc6B_tvalid ;
          wire  [31 : 0]  wacc6B_tdata  ;
          wire            wacc6P_start  ;
          wire            wacc6P_done   ;
          wire            wacc6P_idle   ;
          wire            wacc6P_ready  ;
          wire  [15 : 0]  wacc6P_arg1_V ;
          wire  [15 : 0]  wacc6P_arg2_V ;
          wire  [15 : 0]  wacc6P_arg3_V ;
          wire            wffo6C_tready ;
          wire            wffo6C_tvalid ;
          wire  [31 : 0]  wffo6C_tdata  ;
          wire            wffo6A_tready ;
          wire            wffo6A_tvalid ;
          wire  [31 : 0]  wffo6A_tdata  ;
          wire            wffo6B_tready ;
          wire            wffo6B_tvalid ;
          wire  [31 : 0]  wffo6B_tdata  ;
          wire             wPR6_DONE    ;
          wire            dffo6A_tready ;
          wire            dffo6B_tready ;
          wire            dffo6C_tvalid ;
          wire  [31 : 0]  dffo6C_tdata  ;
          wire            dacc6P_done   ;

          wire            wacc7C_tready ;
          wire            wacc7C_tvalid ;
          wire  [31 : 0]  wacc7C_tdata  ;
          wire            wacc7A_tready ;
          wire            wacc7A_tvalid ;
          wire  [31 : 0]  wacc7A_tdata  ;
          wire            wacc7B_tready ;
          wire            wacc7B_tvalid ;
          wire  [31 : 0]  wacc7B_tdata  ;
          wire            wacc7P_start  ;
          wire            wacc7P_done   ;
          wire            wacc7P_idle   ;
          wire            wacc7P_ready  ;
          wire  [15 : 0]  wacc7P_arg1_V ;
          wire  [15 : 0]  wacc7P_arg2_V ;
          wire  [15 : 0]  wacc7P_arg3_V ;
          wire            wffo7C_tready ;
          wire            wffo7C_tvalid ;
          wire  [31 : 0]  wffo7C_tdata  ;
          wire            wffo7A_tready ;
          wire            wffo7A_tvalid ;
          wire  [31 : 0]  wffo7A_tdata  ;
          wire            wffo7B_tready ;
          wire            wffo7B_tvalid ;
          wire  [31 : 0]  wffo7B_tdata  ;
          wire             wPR7_DONE    ;
          wire            dffo7A_tready ;
          wire            dffo7B_tready ;
          wire            dffo7C_tvalid ;
          wire  [31 : 0]  dffo7C_tdata  ;
          wire            dacc7P_done   ;

          wire            wacc8C_tready ;
          wire            wacc8C_tvalid ;
          wire  [31 : 0]  wacc8C_tdata  ;
          wire            wacc8A_tready ;
          wire            wacc8A_tvalid ;
          wire  [31 : 0]  wacc8A_tdata  ;
          wire            wacc8B_tready ;
          wire            wacc8B_tvalid ;
          wire  [31 : 0]  wacc8B_tdata  ;
          wire            wacc8P_start  ;
          wire            wacc8P_done   ;
          wire            wacc8P_idle   ;
          wire            wacc8P_ready  ;
          wire  [15 : 0]  wacc8P_arg1_V ;
          wire  [15 : 0]  wacc8P_arg2_V ;
          wire  [15 : 0]  wacc8P_arg3_V ;
          wire            wffo8C_tready ;
          wire            wffo8C_tvalid ;
          wire  [31 : 0]  wffo8C_tdata  ;
          wire            wffo8A_tready ;
          wire            wffo8A_tvalid ;
          wire  [31 : 0]  wffo8A_tdata  ;
          wire            wffo8B_tready ;
          wire            wffo8B_tvalid ;
          wire  [31 : 0]  wffo8B_tdata  ;
          wire             wPR8_DONE    ;
          wire            dffo8A_tready ;
          wire            dffo8B_tready ;
          wire            dffo8C_tvalid ;
          wire  [31 : 0]  dffo8C_tdata  ;
          wire            dacc8P_done   ;

  jit_clk u_clk_100MHz  (
    .clk_in1            (clk       ), // Clock in ports 250MHZ
    .clk_out1           (clk_100   ), // Clock out ports  100MHZ for ICAP
    .reset              (rst       ),
    .locked             (clk_locked)
  );

  jit_reset u_rst_100MHz(
    .slowest_sync_clk   (clk_100   ),
    .ext_reset_in       (rst       ),
    .aux_reset_in       (1'b0      ),
    .mb_debug_sys_rst   (1'b0      ),
    .dcm_locked         (clk_locked),
    .peripheral_aresetn (rstn      )
  );

  jit_fifo     u_fifo_s50i (
    .s_axis_tready  ( s50i_rdy     ),
    .s_axis_tvalid  ( s50i_valid   ),
    .s_axis_tdata   ( s50i_data    ),
    .m_axis_tready  (ws50i_rdy     ),
    .m_axis_tvalid  (ws50i_valid   ),
    .m_axis_tdata   (ws50i_data    ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

//jit_fifo     u_fifo_s50o (
//  .s_axis_tready  (ws50o_rdy     ),
//  .s_axis_tvalid  (ws50o_valid   ),
//  .s_axis_tdata   (ws50o_data    ),
//  .m_axis_tready  ( s50o_rdy     ),
//  .m_axis_tvalid  ( s50o_valid   ),
//  .m_axis_tdata   ( s50o_data    ),
//  //--------------(--------------),
//  .s_aclk         (clk_100       ),
//  .m_aclk         (clk           ),
//  .s_aresetn      (rstn          )
//);
//==================================================================================================
//       _    ____ ____       _
//      / \  / ___/ ___|  _  / |
//     / _ \| |  | |     (_) | |
//    / ___ \ |__| |___   _  | |
//   /_/   \_\____\____| (_) |_|
//==================================================================================================
generate if (NUM_ACCs >= 2) begin
  jit_fifo       u_fifo_s11i (
    .s_axis_tready  (   s11i_rdy   ),
    .s_axis_tvalid  (   s11i_valid ),
    .s_axis_tdata   (   s11i_data  ),
    .m_axis_tready  (  ws11i_rdy   ),
    .m_axis_tvalid  (  ws11i_valid ),
    .m_axis_tdata   (  ws11i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s12i (
    .s_axis_tready  (   s12i_rdy   ),
    .s_axis_tvalid  (   s12i_valid ),
    .s_axis_tdata   (   s12i_data  ),
    .m_axis_tready  (  ws12i_rdy   ),
    .m_axis_tvalid  (  ws12i_valid ),
    .m_axis_tdata   (  ws12i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s13o (
    .s_axis_tready  (  ws13o_rdy   ),
    .s_axis_tvalid  (  ws13o_valid ),
    .s_axis_tdata   (  ws13o_data  ),
    .m_axis_tready  (   s13o_rdy   ),
    .m_axis_tvalid  (   s13o_valid ),
    .m_axis_tdata   (   s13o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s14i (
    .s_axis_tready  (   s14i_rdy   ),
    .s_axis_tvalid  (   s14i_valid ),
    .s_axis_tdata   (   s14i_data  ),
    .m_axis_tready  (  ws14i_rdy   ),
    .m_axis_tvalid  (  ws14i_valid ),
    .m_axis_tdata   (  ws14i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s15i (
    .s_axis_tready  (   s15i_rdy   ),
    .s_axis_tvalid  (   s15i_valid ),
    .s_axis_tdata   (   s15i_data  ),
    .m_axis_tready  (  ws15i_rdy   ),
    .m_axis_tvalid  (  ws15i_valid ),
    .m_axis_tdata   (  ws15i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s16o (
    .s_axis_tready  (  ws16o_rdy   ),
    .s_axis_tvalid  (  ws16o_valid ),
    .s_axis_tdata   (  ws16o_data  ),
    .m_axis_tready  (   s16o_rdy   ),
    .m_axis_tvalid  (   s16o_valid ),
    .m_axis_tdata   (   s16o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc1A (
    .s_axis_tready  (wacc1A_tready ),
    .s_axis_tvalid  (wacc1A_tvalid ),
    .s_axis_tdata   (wacc1A_tdata  ),
    .m_axis_tready  (wffo1A_tready ),
    .m_axis_tvalid  (wffo1A_tvalid ),
    .m_axis_tdata   (wffo1A_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc1B (
    .s_axis_tready  (wacc1B_tready ),
    .s_axis_tvalid  (wacc1B_tvalid ),
    .s_axis_tdata   (wacc1B_tdata  ),
    .m_axis_tready  (wffo1B_tready ),
    .m_axis_tvalid  (wffo1B_tvalid ),
    .m_axis_tdata   (wffo1B_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc1C (
    .s_axis_tready  (wffo1C_tready ),
    .s_axis_tvalid  (wffo1C_tvalid ),
    .s_axis_tdata   (wffo1C_tdata  ),
    .m_axis_tready  (wacc1C_tready ),
    .m_axis_tvalid  (wacc1C_tvalid ),
    .m_axis_tdata   (wacc1C_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  prdoor           ACC_PR1_A_rdy   (
    .S              ( wPR1_DONE    ),
    .I              (dffo1A_tready ),
    .O              (wffo1A_tready )
  );

  prdoor           ACC_PR1_B_rdy   (
    .S              ( wPR1_DONE    ),
    .I              (dffo1B_tready ),
    .O              (wffo1B_tready )
  );

  prdoor           ACC_PR1_C_valid (
    .S              ( wPR1_DONE    ),
    .I              (dffo1C_tvalid ),
    .O              (wffo1C_tvalid )
  );

  prdoor #(32)     ACC_PR1_C_data  (
    .S              ( wPR1_DONE    ),
    .I              (dffo1C_tdata  ),
    .O              (wffo1C_tdata  )
  );

  prdoor           ACC_PR1_P_done  (
    .S              ( wPR1_DONE    ),
    .I              (dacc1P_done   ),
    .O              (wacc1P_done   )
  );

  jit_blackbox     ACC_PR1 (
    .mO1_V_TREADY   (wffo1C_tready ),
    .mO1_V_TVALID   (dffo1C_tvalid ),
    .mO1_V_TDATA    (dffo1C_tdata  ),
    .sI1_V_TREADY   (dffo1A_tready ),
    .sI1_V_TVALID   (wffo1A_tvalid ),
    .sI1_V_TDATA    (wffo1A_tdata  ),
    .sI2_V_TREADY   (dffo1B_tready ),
    .sI2_V_TVALID   (wffo1B_tvalid ),
    .sI2_V_TDATA    (wffo1B_tdata  ),
//  .ap_start       (wacc1P_start  ),
//  .ap_done        (dacc1P_done   ),
//  .ap_idle        (wacc1P_idle   ),
//  .ap_ready       (wacc1P_ready  ),
    .arg1_V         (wacc1P_arg1_V ),
    .arg2_V         (wacc1P_arg2_V ),
    .arg3_V         (wacc1P_arg3_V ),
    //--------------(--------------),
    .ap_clk         (clk_100       ),
    .ap_rst_n       (rstn          )
  );

  prctrl        u_prc_PR1 (
    .ID             (4'h1          ),
    .PR_DONE        (wPR1_DONE     ),
    //--------------(--------------),
    .PR_VALID       (ws50i_valid   ),
    .PR_DATA        (ws50i_data    ),
    .clk            (clk_100       ),
    .rstn           (rstn          )
  );
end
endgenerate // NUM_ACCs : 1
//==================================================================================================
//       _    ____ ____       ____
//      / \  / ___/ ___|  _  |___ \
//     / _ \| |  | |     (_)   __) |
//    / ___ \ |__| |___   _   / __/
//   /_/   \_\____\____| (_) |_____|
//==================================================================================================
generate if (NUM_ACCs >= 2) begin
  jit_fifo       u_fifo_s21i (
    .s_axis_tready  (   s21i_rdy   ),
    .s_axis_tvalid  (   s21i_valid ),
    .s_axis_tdata   (   s21i_data  ),
    .m_axis_tready  (  ws21i_rdy   ),
    .m_axis_tvalid  (  ws21i_valid ),
    .m_axis_tdata   (  ws21i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s22i (
    .s_axis_tready  (   s22i_rdy   ),
    .s_axis_tvalid  (   s22i_valid ),
    .s_axis_tdata   (   s22i_data  ),
    .m_axis_tready  (  ws22i_rdy   ),
    .m_axis_tvalid  (  ws22i_valid ),
    .m_axis_tdata   (  ws22i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s23o (
    .s_axis_tready  (  ws23o_rdy   ),
    .s_axis_tvalid  (  ws23o_valid ),
    .s_axis_tdata   (  ws23o_data  ),
    .m_axis_tready  (   s23o_rdy   ),
    .m_axis_tvalid  (   s23o_valid ),
    .m_axis_tdata   (   s23o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s24i (
    .s_axis_tready  (   s24i_rdy   ),
    .s_axis_tvalid  (   s24i_valid ),
    .s_axis_tdata   (   s24i_data  ),
    .m_axis_tready  (  ws24i_rdy   ),
    .m_axis_tvalid  (  ws24i_valid ),
    .m_axis_tdata   (  ws24i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s25i (
    .s_axis_tready  (   s25i_rdy   ),
    .s_axis_tvalid  (   s25i_valid ),
    .s_axis_tdata   (   s25i_data  ),
    .m_axis_tready  (  ws25i_rdy   ),
    .m_axis_tvalid  (  ws25i_valid ),
    .m_axis_tdata   (  ws25i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s26o (
    .s_axis_tready  (  ws26o_rdy   ),
    .s_axis_tvalid  (  ws26o_valid ),
    .s_axis_tdata   (  ws26o_data  ),
    .m_axis_tready  (   s26o_rdy   ),
    .m_axis_tvalid  (   s26o_valid ),
    .m_axis_tdata   (   s26o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc2A (
    .s_axis_tready  (wacc2A_tready ),
    .s_axis_tvalid  (wacc2A_tvalid ),
    .s_axis_tdata   (wacc2A_tdata  ),
    .m_axis_tready  (wffo2A_tready ),
    .m_axis_tvalid  (wffo2A_tvalid ),
    .m_axis_tdata   (wffo2A_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc2B (
    .s_axis_tready  (wacc2B_tready ),
    .s_axis_tvalid  (wacc2B_tvalid ),
    .s_axis_tdata   (wacc2B_tdata  ),
    .m_axis_tready  (wffo2B_tready ),
    .m_axis_tvalid  (wffo2B_tvalid ),
    .m_axis_tdata   (wffo2B_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc2C (
    .s_axis_tready  (wffo2C_tready ),
    .s_axis_tvalid  (wffo2C_tvalid ),
    .s_axis_tdata   (wffo2C_tdata  ),
    .m_axis_tready  (wacc2C_tready ),
    .m_axis_tvalid  (wacc2C_tvalid ),
    .m_axis_tdata   (wacc2C_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  prdoor           ACC_PR2_A_rdy   (
    .S              ( wPR2_DONE    ),
    .I              (dffo2A_tready ),
    .O              (wffo2A_tready )
  );

  prdoor           ACC_PR2_B_rdy   (
    .S              ( wPR2_DONE    ),
    .I              (dffo2B_tready ),
    .O              (wffo2B_tready )
  );

  prdoor           ACC_PR2_C_valid (
    .S              ( wPR2_DONE    ),
    .I              (dffo2C_tvalid ),
    .O              (wffo2C_tvalid )
  );

  prdoor #(32)     ACC_PR2_C_data  (
    .S              ( wPR2_DONE    ),
    .I              (dffo2C_tdata  ),
    .O              (wffo2C_tdata  )
  );

  prdoor           ACC_PR2_P_done  (
    .S              ( wPR2_DONE    ),
    .I              (dacc2P_done   ),
    .O              (wacc2P_done   )
  );

  jit_blackbox     ACC_PR2 (
    .mO1_V_TREADY   (wffo2C_tready ),
    .mO1_V_TVALID   (dffo2C_tvalid ),
    .mO1_V_TDATA    (dffo2C_tdata  ),
    .sI1_V_TREADY   (dffo2A_tready ),
    .sI1_V_TVALID   (wffo2A_tvalid ),
    .sI1_V_TDATA    (wffo2A_tdata  ),
    .sI2_V_TREADY   (dffo2B_tready ),
    .sI2_V_TVALID   (wffo2B_tvalid ),
    .sI2_V_TDATA    (wffo2B_tdata  ),
//  .ap_start       (wacc2P_start  ),
//  .ap_done        (dacc2P_done   ),
//  .ap_idle        (wacc2P_idle   ),
//  .ap_ready       (wacc2P_ready  ),
    .arg1_V         (wacc2P_arg1_V ),
    .arg2_V         (wacc2P_arg2_V ),
    .arg3_V         (wacc2P_arg3_V ),
    //--------------(--------------),
    .ap_clk         (clk_100       ),
    .ap_rst_n       (rstn          )
  );

  prctrl        u_prc_PR2 (
    .ID             (4'h2          ),
    .PR_DONE        (wPR2_DONE     ),
    //--------------(--------------),
    .PR_VALID       (ws50i_valid   ),
    .PR_DATA        (ws50i_data    ),
    .clk            (clk_100       ),
    .rstn           (rstn          )
  );
end
endgenerate // NUM_ACCs : 2
//==============================================================================
//       _    ____ ____       _____
//      / \  / ___/ ___|  _  |___ /
//     / _ \| |  | |     (_)   |_ \
//    / ___ \ |__| |___   _   ___) |
//   /_/   \_\____\____| (_) |____/
//==============================================================================
generate if (NUM_ACCs >= 3) begin
  jit_fifo       u_fifo_s31i (
    .s_axis_tready  (   s31i_rdy   ),
    .s_axis_tvalid  (   s31i_valid ),
    .s_axis_tdata   (   s31i_data  ),
    .m_axis_tready  (  ws31i_rdy   ),
    .m_axis_tvalid  (  ws31i_valid ),
    .m_axis_tdata   (  ws31i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s32i (
    .s_axis_tready  (   s32i_rdy   ),
    .s_axis_tvalid  (   s32i_valid ),
    .s_axis_tdata   (   s32i_data  ),
    .m_axis_tready  (  ws32i_rdy   ),
    .m_axis_tvalid  (  ws32i_valid ),
    .m_axis_tdata   (  ws32i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s33o (
    .s_axis_tready  (  ws33o_rdy   ),
    .s_axis_tvalid  (  ws33o_valid ),
    .s_axis_tdata   (  ws33o_data  ),
    .m_axis_tready  (   s33o_rdy   ),
    .m_axis_tvalid  (   s33o_valid ),
    .m_axis_tdata   (   s33o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s34i (
    .s_axis_tready  (   s34i_rdy   ),
    .s_axis_tvalid  (   s34i_valid ),
    .s_axis_tdata   (   s34i_data  ),
    .m_axis_tready  (  ws34i_rdy   ),
    .m_axis_tvalid  (  ws34i_valid ),
    .m_axis_tdata   (  ws34i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s35i (
    .s_axis_tready  (   s35i_rdy   ),
    .s_axis_tvalid  (   s35i_valid ),
    .s_axis_tdata   (   s35i_data  ),
    .m_axis_tready  (  ws35i_rdy   ),
    .m_axis_tvalid  (  ws35i_valid ),
    .m_axis_tdata   (  ws35i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s36o (
    .s_axis_tready  (  ws36o_rdy   ),
    .s_axis_tvalid  (  ws36o_valid ),
    .s_axis_tdata   (  ws36o_data  ),
    .m_axis_tready  (   s36o_rdy   ),
    .m_axis_tvalid  (   s36o_valid ),
    .m_axis_tdata   (   s36o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc3A (
    .s_axis_tready  (wacc3A_tready ),
    .s_axis_tvalid  (wacc3A_tvalid ),
    .s_axis_tdata   (wacc3A_tdata  ),
    .m_axis_tready  (wffo3A_tready ),
    .m_axis_tvalid  (wffo3A_tvalid ),
    .m_axis_tdata   (wffo3A_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc3B (
    .s_axis_tready  (wacc3B_tready ),
    .s_axis_tvalid  (wacc3B_tvalid ),
    .s_axis_tdata   (wacc3B_tdata  ),
    .m_axis_tready  (wffo3B_tready ),
    .m_axis_tvalid  (wffo3B_tvalid ),
    .m_axis_tdata   (wffo3B_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc3C (
    .s_axis_tready  (wffo3C_tready ),
    .s_axis_tvalid  (wffo3C_tvalid ),
    .s_axis_tdata   (wffo3C_tdata  ),
    .m_axis_tready  (wacc3C_tready ),
    .m_axis_tvalid  (wacc3C_tvalid ),
    .m_axis_tdata   (wacc3C_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  prdoor           ACC_PR3_A_rdy   (
    .S              ( wPR3_DONE    ),
    .I              (dffo3A_tready ),
    .O              (wffo3A_tready )
  );

  prdoor           ACC_PR3_B_rdy   (
    .S              ( wPR3_DONE    ),
    .I              (dffo3B_tready ),
    .O              (wffo3B_tready )
  );

  prdoor           ACC_PR3_C_valid (
    .S              ( wPR3_DONE    ),
    .I              (dffo3C_tvalid ),
    .O              (wffo3C_tvalid )
  );

  prdoor #(32)     ACC_PR3_C_data  (
    .S              ( wPR3_DONE    ),
    .I              (dffo3C_tdata  ),
    .O              (wffo3C_tdata  )
  );

  prdoor           ACC_PR3_P_done  (
    .S              ( wPR3_DONE    ),
    .I              (dacc3P_done   ),
    .O              (wacc3P_done   )
  );

  jit_blackbox     ACC_PR3 (
    .mO1_V_TREADY   (wffo3C_tready ),
    .mO1_V_TVALID   (dffo3C_tvalid ),
    .mO1_V_TDATA    (dffo3C_tdata  ),
    .sI1_V_TREADY   (dffo3A_tready ),
    .sI1_V_TVALID   (wffo3A_tvalid ),
    .sI1_V_TDATA    (wffo3A_tdata  ),
    .sI2_V_TREADY   (dffo3B_tready ),
    .sI2_V_TVALID   (wffo3B_tvalid ),
    .sI2_V_TDATA    (wffo3B_tdata  ),
//  .ap_start       (wacc3P_start  ),
//  .ap_done        (dacc3P_done   ),
//  .ap_idle        (wacc3P_idle   ),
//  .ap_ready       (wacc3P_ready  ),
    .arg1_V         (wacc3P_arg1_V ),
    .arg2_V         (wacc3P_arg2_V ),
    .arg3_V         (wacc3P_arg3_V ),
    //--------------(--------------),
    .ap_clk         (clk_100       ),
    .ap_rst_n       (rstn          )
  );

  prctrl         u_prc_PR3 (
    .ID             ( 4'h3         ),
    .PR_DONE        ( wPR3_DONE    ),
    //--------------(--------------),
    .PR_VALID       (ws50i_valid   ),
    .PR_DATA        (ws50i_data    ),
    .clk            (clk_100       ),
    .rstn           (rstn          )
  );
end
endgenerate // NUM_ACCs : 3
//==============================================================================
//       _    ____ ____       _  _
//      / \  / ___/ ___|  _  | || |
//     / _ \| |  | |     (_) | || |_
//    / ___ \ |__| |___   _  |__   _|
//   /_/   \_\____\____| (_)    |_|
//==============================================================================
generate if (NUM_ACCs >= 4) begin
  jit_fifo       u_fifo_s41i (
    .s_axis_tready  (   s41i_rdy   ),
    .s_axis_tvalid  (   s41i_valid ),
    .s_axis_tdata   (   s41i_data  ),
    .m_axis_tready  (  ws41i_rdy   ),
    .m_axis_tvalid  (  ws41i_valid ),
    .m_axis_tdata   (  ws41i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s42i (
    .s_axis_tready  (   s42i_rdy   ),
    .s_axis_tvalid  (   s42i_valid ),
    .s_axis_tdata   (   s42i_data  ),
    .m_axis_tready  (  ws42i_rdy   ),
    .m_axis_tvalid  (  ws42i_valid ),
    .m_axis_tdata   (  ws42i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s43o (
    .s_axis_tready  (  ws43o_rdy   ),
    .s_axis_tvalid  (  ws43o_valid ),
    .s_axis_tdata   (  ws43o_data  ),
    .m_axis_tready  (   s43o_rdy   ),
    .m_axis_tvalid  (   s43o_valid ),
    .m_axis_tdata   (   s43o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s44i (
    .s_axis_tready  (   s44i_rdy   ),
    .s_axis_tvalid  (   s44i_valid ),
    .s_axis_tdata   (   s44i_data  ),
    .m_axis_tready  (  ws44i_rdy   ),
    .m_axis_tvalid  (  ws44i_valid ),
    .m_axis_tdata   (  ws44i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s45i (
    .s_axis_tready  (   s45i_rdy   ),
    .s_axis_tvalid  (   s45i_valid ),
    .s_axis_tdata   (   s45i_data  ),
    .m_axis_tready  (  ws45i_rdy   ),
    .m_axis_tvalid  (  ws45i_valid ),
    .m_axis_tdata   (  ws45i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s46o (
    .s_axis_tready  (  ws46o_rdy   ),
    .s_axis_tvalid  (  ws46o_valid ),
    .s_axis_tdata   (  ws46o_data  ),
    .m_axis_tready  (   s46o_rdy   ),
    .m_axis_tvalid  (   s46o_valid ),
    .m_axis_tdata   (   s46o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc4A (
    .s_axis_tready  (wacc4A_tready ),
    .s_axis_tvalid  (wacc4A_tvalid ),
    .s_axis_tdata   (wacc4A_tdata  ),
    .m_axis_tready  (wffo4A_tready ),
    .m_axis_tvalid  (wffo4A_tvalid ),
    .m_axis_tdata   (wffo4A_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc4B (
    .s_axis_tready  (wacc4B_tready ),
    .s_axis_tvalid  (wacc4B_tvalid ),
    .s_axis_tdata   (wacc4B_tdata  ),
    .m_axis_tready  (wffo4B_tready ),
    .m_axis_tvalid  (wffo4B_tvalid ),
    .m_axis_tdata   (wffo4B_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc4C (
    .s_axis_tready  (wffo4C_tready ),
    .s_axis_tvalid  (wffo4C_tvalid ),
    .s_axis_tdata   (wffo4C_tdata  ),
    .m_axis_tready  (wacc4C_tready ),
    .m_axis_tvalid  (wacc4C_tvalid ),
    .m_axis_tdata   (wacc4C_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  prdoor           ACC_PR4_A_rdy   (
    .S              ( wPR4_DONE    ),
    .I              (dffo4A_tready ),
    .O              (wffo4A_tready )
  );

  prdoor           ACC_PR4_B_rdy   (
    .S              ( wPR4_DONE    ),
    .I              (dffo4B_tready ),
    .O              (wffo4B_tready )
  );

  prdoor           ACC_PR4_C_valid (
    .S              ( wPR4_DONE    ),
    .I              (dffo4C_tvalid ),
    .O              (wffo4C_tvalid )
  );

  prdoor #(32)     ACC_PR4_C_data  (
    .S              ( wPR4_DONE    ),
    .I              (dffo4C_tdata  ),
    .O              (wffo4C_tdata  )
  );

  prdoor           ACC_PR4_P_done  (
    .S              ( wPR4_DONE    ),
    .I              (dacc4P_done   ),
    .O              (wacc4P_done   )
  );

  jit_blackbox     ACC_PR4 (
    .mO1_V_TREADY   (wffo4C_tready ),
    .mO1_V_TVALID   (dffo4C_tvalid ),
    .mO1_V_TDATA    (dffo4C_tdata  ),
    .sI1_V_TREADY   (dffo4A_tready ),
    .sI1_V_TVALID   (wffo4A_tvalid ),
    .sI1_V_TDATA    (wffo4A_tdata  ),
    .sI2_V_TREADY   (dffo4B_tready ),
    .sI2_V_TVALID   (wffo4B_tvalid ),
    .sI2_V_TDATA    (wffo4B_tdata  ),
//  .ap_start       (wacc4P_start  ),
//  .ap_done        (dacc4P_done   ),
//  .ap_idle        (wacc4P_idle   ),
//  .ap_ready       (wacc4P_ready  ),
    .arg1_V         (wacc4P_arg1_V ),
    .arg2_V         (wacc4P_arg2_V ),
    .arg3_V         (wacc4P_arg3_V ),
    //--------------(--------------),
    .ap_clk         (clk_100       ),
    .ap_rst_n       (rstn          )
  );

  prctrl         u_prc_PR4 (
    .ID             ( 4'h4         ),
    .PR_DONE        ( wPR4_DONE    ),
    //--------------(--------------),
    .PR_VALID       (ws50i_valid   ),
    .PR_DATA        (ws50i_data    ),
    .clk            (clk_100       ),
    .rstn           (rstn          )
  );
end
endgenerate // NUM_ACCs : 4
//==============================================================================
//       _    ____ ____       ____
//      / \  / ___/ ___|  _  | ___|
//     / _ \| |  | |     (_) |___ \
//    / ___ \ |__| |___   _   ___) |
//   /_/   \_\____\____| (_) |____/
//==============================================================================
generate if (NUM_ACCs >= 5) begin
  jit_fifo       u_fifo_s51i (
    .s_axis_tready  (   s51i_rdy   ),
    .s_axis_tvalid  (   s51i_valid ),
    .s_axis_tdata   (   s51i_data  ),
    .m_axis_tready  (  ws51i_rdy   ),
    .m_axis_tvalid  (  ws51i_valid ),
    .m_axis_tdata   (  ws51i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s52i (
    .s_axis_tready  (   s52i_rdy   ),
    .s_axis_tvalid  (   s52i_valid ),
    .s_axis_tdata   (   s52i_data  ),
    .m_axis_tready  (  ws52i_rdy   ),
    .m_axis_tvalid  (  ws52i_valid ),
    .m_axis_tdata   (  ws52i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s53o (
    .s_axis_tready  (  ws53o_rdy   ),
    .s_axis_tvalid  (  ws53o_valid ),
    .s_axis_tdata   (  ws53o_data  ),
    .m_axis_tready  (   s53o_rdy   ),
    .m_axis_tvalid  (   s53o_valid ),
    .m_axis_tdata   (   s53o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s54i (
    .s_axis_tready  (   s54i_rdy   ),
    .s_axis_tvalid  (   s54i_valid ),
    .s_axis_tdata   (   s54i_data  ),
    .m_axis_tready  (  ws54i_rdy   ),
    .m_axis_tvalid  (  ws54i_valid ),
    .m_axis_tdata   (  ws54i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s55i (
    .s_axis_tready  (   s55i_rdy   ),
    .s_axis_tvalid  (   s55i_valid ),
    .s_axis_tdata   (   s55i_data  ),
    .m_axis_tready  (  ws55i_rdy   ),
    .m_axis_tvalid  (  ws55i_valid ),
    .m_axis_tdata   (  ws55i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s56o (
    .s_axis_tready  (  ws56o_rdy   ),
    .s_axis_tvalid  (  ws56o_valid ),
    .s_axis_tdata   (  ws56o_data  ),
    .m_axis_tready  (   s56o_rdy   ),
    .m_axis_tvalid  (   s56o_valid ),
    .m_axis_tdata   (   s56o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc5A (
    .s_axis_tready  (wacc5A_tready ),
    .s_axis_tvalid  (wacc5A_tvalid ),
    .s_axis_tdata   (wacc5A_tdata  ),
    .m_axis_tready  (wffo5A_tready ),
    .m_axis_tvalid  (wffo5A_tvalid ),
    .m_axis_tdata   (wffo5A_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc5B (
    .s_axis_tready  (wacc5B_tready ),
    .s_axis_tvalid  (wacc5B_tvalid ),
    .s_axis_tdata   (wacc5B_tdata  ),
    .m_axis_tready  (wffo5B_tready ),
    .m_axis_tvalid  (wffo5B_tvalid ),
    .m_axis_tdata   (wffo5B_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc5C (
    .s_axis_tready  (wffo5C_tready ),
    .s_axis_tvalid  (wffo5C_tvalid ),
    .s_axis_tdata   (wffo5C_tdata  ),
    .m_axis_tready  (wacc5C_tready ),
    .m_axis_tvalid  (wacc5C_tvalid ),
    .m_axis_tdata   (wacc5C_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  prdoor           ACC_PR5_A_rdy   (
    .S              ( wPR5_DONE    ),
    .I              (dffo5A_tready ),
    .O              (wffo5A_tready )
  );

  prdoor           ACC_PR5_B_rdy   (
    .S              ( wPR5_DONE    ),
    .I              (dffo5B_tready ),
    .O              (wffo5B_tready )
  );

  prdoor           ACC_PR5_C_valid (
    .S              ( wPR5_DONE    ),
    .I              (dffo5C_tvalid ),
    .O              (wffo5C_tvalid )
  );

  prdoor #(32)     ACC_PR5_C_data  (
    .S              ( wPR5_DONE    ),
    .I              (dffo5C_tdata  ),
    .O              (wffo5C_tdata  )
  );

  prdoor           ACC_PR5_P_done  (
    .S              ( wPR5_DONE    ),
    .I              (dacc5P_done   ),
    .O              (wacc5P_done   )
  );

  jit_blackbox      ACC_PR5 (
    .mO1_V_TREADY   (wffo5C_tready ),
    .mO1_V_TVALID   (dffo5C_tvalid ),
    .mO1_V_TDATA    (dffo5C_tdata  ),
    .sI1_V_TREADY   (dffo5A_tready ),
    .sI1_V_TVALID   (wffo5A_tvalid ),
    .sI1_V_TDATA    (wffo5A_tdata  ),
    .sI2_V_TREADY   (dffo5B_tready ),
    .sI2_V_TVALID   (wffo5B_tvalid ),
    .sI2_V_TDATA    (wffo5B_tdata  ),
//  .ap_start       (wacc5P_start  ),
//  .ap_done        (dacc5P_done   ),
//  .ap_idle        (wacc5P_idle   ),
//  .ap_ready       (wacc5P_ready  ),
    .arg1_V         (wacc5P_arg1_V ),
    .arg2_V         (wacc5P_arg2_V ),
    .arg3_V         (wacc5P_arg3_V ),
    //--------------(--------------),
    .ap_clk         (clk_100       ),
    .ap_rst_n       (rstn          )
  );

  prctrl         u_prc_PR5 (
    .ID             ( 4'h5         ),
    .PR_DONE        ( wPR5_DONE    ),
    //--------------(--------------),
    .PR_VALID       (ws50i_valid   ),
    .PR_DATA        (ws50i_data    ),
    .clk            (clk_100       ),
    .rstn           (rstn          )
  );
end
endgenerate // NUM_ACCs : 5
//==============================================================================
//       _    ____ ____        __
//      / \  / ___/ ___|  _   / /_
//     / _ \| |  | |     (_) | '_ \
//    / ___ \ |__| |___   _  | (_) |
//   /_/   \_\____\____| (_)  \___/
//==============================================================================
generate if (NUM_ACCs >= 6) begin
  jit_fifo       u_fifo_s61i (
    .s_axis_tready  (   s61i_rdy   ),
    .s_axis_tvalid  (   s61i_valid ),
    .s_axis_tdata   (   s61i_data  ),
    .m_axis_tready  (  ws61i_rdy   ),
    .m_axis_tvalid  (  ws61i_valid ),
    .m_axis_tdata   (  ws61i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s62i (
    .s_axis_tready  (   s62i_rdy   ),
    .s_axis_tvalid  (   s62i_valid ),
    .s_axis_tdata   (   s62i_data  ),
    .m_axis_tready  (  ws62i_rdy   ),
    .m_axis_tvalid  (  ws62i_valid ),
    .m_axis_tdata   (  ws62i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s63o (
    .s_axis_tready  (  ws63o_rdy   ),
    .s_axis_tvalid  (  ws63o_valid ),
    .s_axis_tdata   (  ws63o_data  ),
    .m_axis_tready  (   s63o_rdy   ),
    .m_axis_tvalid  (   s63o_valid ),
    .m_axis_tdata   (   s63o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s64i (
    .s_axis_tready  (   s64i_rdy   ),
    .s_axis_tvalid  (   s64i_valid ),
    .s_axis_tdata   (   s64i_data  ),
    .m_axis_tready  (  ws64i_rdy   ),
    .m_axis_tvalid  (  ws64i_valid ),
    .m_axis_tdata   (  ws64i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s65i (
    .s_axis_tready  (   s65i_rdy   ),
    .s_axis_tvalid  (   s65i_valid ),
    .s_axis_tdata   (   s65i_data  ),
    .m_axis_tready  (  ws65i_rdy   ),
    .m_axis_tvalid  (  ws65i_valid ),
    .m_axis_tdata   (  ws65i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s66o (
    .s_axis_tready  (  ws66o_rdy   ),
    .s_axis_tvalid  (  ws66o_valid ),
    .s_axis_tdata   (  ws66o_data  ),
    .m_axis_tready  (   s66o_rdy   ),
    .m_axis_tvalid  (   s66o_valid ),
    .m_axis_tdata   (   s66o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc6A (
    .s_axis_tready  (wacc6A_tready ),
    .s_axis_tvalid  (wacc6A_tvalid ),
    .s_axis_tdata   (wacc6A_tdata  ),
    .m_axis_tready  (wffo6A_tready ),
    .m_axis_tvalid  (wffo6A_tvalid ),
    .m_axis_tdata   (wffo6A_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc6B (
    .s_axis_tready  (wacc6B_tready ),
    .s_axis_tvalid  (wacc6B_tvalid ),
    .s_axis_tdata   (wacc6B_tdata  ),
    .m_axis_tready  (wffo6B_tready ),
    .m_axis_tvalid  (wffo6B_tvalid ),
    .m_axis_tdata   (wffo6B_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc6C (
    .s_axis_tready  (wffo6C_tready ),
    .s_axis_tvalid  (wffo6C_tvalid ),
    .s_axis_tdata   (wffo6C_tdata  ),
    .m_axis_tready  (wacc6C_tready ),
    .m_axis_tvalid  (wacc6C_tvalid ),
    .m_axis_tdata   (wacc6C_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  prdoor           ACC_PR6_A_rdy   (
    .S              ( wPR6_DONE    ),
    .I              (dffo6A_tready ),
    .O              (wffo6A_tready )
  );

  prdoor           ACC_PR6_B_rdy   (
    .S              ( wPR6_DONE    ),
    .I              (dffo6B_tready ),
    .O              (wffo6B_tready )
  );

  prdoor           ACC_PR6_C_valid (
    .S              ( wPR6_DONE    ),
    .I              (dffo6C_tvalid ),
    .O              (wffo6C_tvalid )
  );

  prdoor #(32)     ACC_PR6_C_data  (
    .S              ( wPR6_DONE    ),
    .I              (dffo6C_tdata  ),
    .O              (wffo6C_tdata  )
  );

  prdoor           ACC_PR6_P_done  (
    .S              ( wPR6_DONE    ),
    .I              (dacc6P_done   ),
    .O              (wacc6P_done   )
  );

  jit_blackbox     ACC_PR6 (
    .mO1_V_TREADY   (wffo6C_tready ),
    .mO1_V_TVALID   (dffo6C_tvalid ),
    .mO1_V_TDATA    (dffo6C_tdata  ),
    .sI1_V_TREADY   (dffo6A_tready ),
    .sI1_V_TVALID   (wffo6A_tvalid ),
    .sI1_V_TDATA    (wffo6A_tdata  ),
    .sI2_V_TREADY   (dffo6B_tready ),
    .sI2_V_TVALID   (wffo6B_tvalid ),
    .sI2_V_TDATA    (wffo6B_tdata  ),
//  .ap_start       (wacc6P_start  ),
//  .ap_done        (dacc6P_done   ),
//  .ap_idle        (wacc6P_idle   ),
//  .ap_ready       (wacc6P_ready  ),
    .arg1_V         (wacc6P_arg1_V ),
    .arg2_V         (wacc6P_arg2_V ),
    .arg3_V         (wacc6P_arg3_V ),
    //--------------(--------------),
    .ap_clk         (clk_100       ),
    .ap_rst_n       (rstn          )
  );

  prctrl         u_prc_PR6 (
    .ID             ( 4'h6         ),
    .PR_DONE        ( wPR6_DONE    ),
    //--------------(--------------),
    .PR_VALID       (ws50i_valid   ),
    .PR_DATA        (ws50i_data    ),
    .clk            (clk_100       ),
    .rstn           (rstn          )
  );
end
endgenerate // NUM_ACCs : 6
//==============================================================================
//       _    ____ ____       _____
//      / \  / ___/ ___|  _  |___  |
//     / _ \| |  | |     (_)    / /
//    / ___ \ |__| |___   _    / /
//   /_/   \_\____\____| (_)  /_/
//==============================================================================
generate if (NUM_ACCs >= 7) begin
  jit_fifo       u_fifo_s71i (
    .s_axis_tready  (   s71i_rdy   ),
    .s_axis_tvalid  (   s71i_valid ),
    .s_axis_tdata   (   s71i_data  ),
    .m_axis_tready  (  ws71i_rdy   ),
    .m_axis_tvalid  (  ws71i_valid ),
    .m_axis_tdata   (  ws71i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s72i (
    .s_axis_tready  (   s72i_rdy   ),
    .s_axis_tvalid  (   s72i_valid ),
    .s_axis_tdata   (   s72i_data  ),
    .m_axis_tready  (  ws72i_rdy   ),
    .m_axis_tvalid  (  ws72i_valid ),
    .m_axis_tdata   (  ws72i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s73o (
    .s_axis_tready  (  ws73o_rdy   ),
    .s_axis_tvalid  (  ws73o_valid ),
    .s_axis_tdata   (  ws73o_data  ),
    .m_axis_tready  (   s73o_rdy   ),
    .m_axis_tvalid  (   s73o_valid ),
    .m_axis_tdata   (   s73o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s74i (
    .s_axis_tready  (   s74i_rdy   ),
    .s_axis_tvalid  (   s74i_valid ),
    .s_axis_tdata   (   s74i_data  ),
    .m_axis_tready  (  ws74i_rdy   ),
    .m_axis_tvalid  (  ws74i_valid ),
    .m_axis_tdata   (  ws74i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s75i (
    .s_axis_tready  (   s75i_rdy   ),
    .s_axis_tvalid  (   s75i_valid ),
    .s_axis_tdata   (   s75i_data  ),
    .m_axis_tready  (  ws75i_rdy   ),
    .m_axis_tvalid  (  ws75i_valid ),
    .m_axis_tdata   (  ws75i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s76o (
    .s_axis_tready  (  ws76o_rdy   ),
    .s_axis_tvalid  (  ws76o_valid ),
    .s_axis_tdata   (  ws76o_data  ),
    .m_axis_tready  (   s76o_rdy   ),
    .m_axis_tvalid  (   s76o_valid ),
    .m_axis_tdata   (   s76o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc7A (
    .s_axis_tready  (wacc7A_tready ),
    .s_axis_tvalid  (wacc7A_tvalid ),
    .s_axis_tdata   (wacc7A_tdata  ),
    .m_axis_tready  (wffo7A_tready ),
    .m_axis_tvalid  (wffo7A_tvalid ),
    .m_axis_tdata   (wffo7A_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc7B (
    .s_axis_tready  (wacc7B_tready ),
    .s_axis_tvalid  (wacc7B_tvalid ),
    .s_axis_tdata   (wacc7B_tdata  ),
    .m_axis_tready  (wffo7B_tready ),
    .m_axis_tvalid  (wffo7B_tvalid ),
    .m_axis_tdata   (wffo7B_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc7C (
    .s_axis_tready  (wffo7C_tready ),
    .s_axis_tvalid  (wffo7C_tvalid ),
    .s_axis_tdata   (wffo7C_tdata  ),
    .m_axis_tready  (wacc7C_tready ),
    .m_axis_tvalid  (wacc7C_tvalid ),
    .m_axis_tdata   (wacc7C_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  prdoor           ACC_PR7_A_rdy   (
    .S              ( wPR7_DONE    ),
    .I              (dffo7A_tready ),
    .O              (wffo7A_tready )
  );

  prdoor           ACC_PR7_B_rdy   (
    .S              ( wPR7_DONE    ),
    .I              (dffo7B_tready ),
    .O              (wffo7B_tready )
  );

  prdoor           ACC_PR7_C_valid (
    .S              ( wPR7_DONE    ),
    .I              (dffo7C_tvalid ),
    .O              (wffo7C_tvalid )
  );

  prdoor #(32)     ACC_PR7_C_data  (
    .S              ( wPR7_DONE    ),
    .I              (dffo7C_tdata  ),
    .O              (wffo7C_tdata  )
  );

  prdoor           ACC_PR7_P_done  (
    .S              ( wPR7_DONE    ),
    .I              (dacc7P_done   ),
    .O              (wacc7P_done   )
  );

  jit_blackbox     ACC_PR7 (
    .mO1_V_TREADY   (wffo7C_tready ),
    .mO1_V_TVALID   (dffo7C_tvalid ),
    .mO1_V_TDATA    (dffo7C_tdata  ),
    .sI1_V_TREADY   (dffo7A_tready ),
    .sI1_V_TVALID   (wffo7A_tvalid ),
    .sI1_V_TDATA    (wffo7A_tdata  ),
    .sI2_V_TREADY   (dffo7B_tready ),
    .sI2_V_TVALID   (wffo7B_tvalid ),
    .sI2_V_TDATA    (wffo7B_tdata  ),
//  .ap_start       (wacc7P_start  ),
//  .ap_done        (dacc7P_done   ),
//  .ap_idle        (wacc7P_idle   ),
//  .ap_ready       (wacc7P_ready  ),
    .arg1_V         (wacc7P_arg1_V ),
    .arg2_V         (wacc7P_arg2_V ),
    .arg3_V         (wacc7P_arg3_V ),
    //--------------(--------------),
    .ap_clk         (clk_100       ),
    .ap_rst_n       (rstn          )
  );

  prctrl         u_prc_PR7 (
    .ID             ( 4'h7         ),
    .PR_DONE        ( wPR7_DONE    ),
    //--------------(--------------),
    .PR_VALID       (ws50i_valid   ),
    .PR_DATA        (ws50i_data    ),
    .clk            (clk_100       ),
    .rstn           (rstn          )
  );
end
endgenerate // NUM_ACCs : 7
//==============================================================================
//       _    ____ ____        ___
//      / \  / ___/ ___|  _   ( _ )
//     / _ \| |  | |     (_)  / _ \
//    / ___ \ |__| |___   _  | (_) |
//   /_/   \_\____\____| (_)  \___/
//==============================================================================
generate if (NUM_ACCs >= 8) begin
  jit_fifo       u_fifo_s81i (
    .s_axis_tready  (   s81i_rdy   ),
    .s_axis_tvalid  (   s81i_valid ),
    .s_axis_tdata   (   s81i_data  ),
    .m_axis_tready  (  ws81i_rdy   ),
    .m_axis_tvalid  (  ws81i_valid ),
    .m_axis_tdata   (  ws81i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s82i (
    .s_axis_tready  (   s82i_rdy   ),
    .s_axis_tvalid  (   s82i_valid ),
    .s_axis_tdata   (   s82i_data  ),
    .m_axis_tready  (  ws82i_rdy   ),
    .m_axis_tvalid  (  ws82i_valid ),
    .m_axis_tdata   (  ws82i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s83o (
    .s_axis_tready  (  ws83o_rdy   ),
    .s_axis_tvalid  (  ws83o_valid ),
    .s_axis_tdata   (  ws83o_data  ),
    .m_axis_tready  (   s83o_rdy   ),
    .m_axis_tvalid  (   s83o_valid ),
    .m_axis_tdata   (   s83o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s84i (
    .s_axis_tready  (   s84i_rdy   ),
    .s_axis_tvalid  (   s84i_valid ),
    .s_axis_tdata   (   s84i_data  ),
    .m_axis_tready  (  ws84i_rdy   ),
    .m_axis_tvalid  (  ws84i_valid ),
    .m_axis_tdata   (  ws84i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s85i (
    .s_axis_tready  (   s85i_rdy   ),
    .s_axis_tvalid  (   s85i_valid ),
    .s_axis_tdata   (   s85i_data  ),
    .m_axis_tready  (  ws85i_rdy   ),
    .m_axis_tvalid  (  ws85i_valid ),
    .m_axis_tdata   (  ws85i_data  ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo       u_fifo_s86o (
    .s_axis_tready  (  ws86o_rdy   ),
    .s_axis_tvalid  (  ws86o_valid ),
    .s_axis_tdata   (  ws86o_data  ),
    .m_axis_tready  (   s86o_rdy   ),
    .m_axis_tvalid  (   s86o_valid ),
    .m_axis_tdata   (   s86o_data  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk           ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc8A (
    .s_axis_tready  (wacc8A_tready ),
    .s_axis_tvalid  (wacc8A_tvalid ),
    .s_axis_tdata   (wacc8A_tdata  ),
    .m_axis_tready  (wffo8A_tready ),
    .m_axis_tvalid  (wffo8A_tvalid ),
    .m_axis_tdata   (wffo8A_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc8B (
    .s_axis_tready  (wacc8B_tready ),
    .s_axis_tvalid  (wacc8B_tvalid ),
    .s_axis_tdata   (wacc8B_tdata  ),
    .m_axis_tready  (wffo8B_tready ),
    .m_axis_tvalid  (wffo8B_tvalid ),
    .m_axis_tdata   (wffo8B_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  jit_fifo     u_fifo_acc8C (
    .s_axis_tready  (wffo8C_tready ),
    .s_axis_tvalid  (wffo8C_tvalid ),
    .s_axis_tdata   (wffo8C_tdata  ),
    .m_axis_tready  (wacc8C_tready ),
    .m_axis_tvalid  (wacc8C_tvalid ),
    .m_axis_tdata   (wacc8C_tdata  ),
    //--------------(--------------),
    .s_aclk         (clk_100       ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  prdoor           ACC_PR8_A_rdy   (
    .S              ( wPR8_DONE    ),
    .I              (dffo8A_tready ),
    .O              (wffo8A_tready )
  );

  prdoor           ACC_PR8_B_rdy   (
    .S              ( wPR8_DONE    ),
    .I              (dffo8B_tready ),
    .O              (wffo8B_tready )
  );

  prdoor           ACC_PR8_C_valid (
    .S              ( wPR8_DONE    ),
    .I              (dffo8C_tvalid ),
    .O              (wffo8C_tvalid )
  );

  prdoor #(32)     ACC_PR8_C_data  (
    .S              ( wPR8_DONE    ),
    .I              (dffo8C_tdata  ),
    .O              (wffo8C_tdata  )
  );

  prdoor           ACC_PR8_P_done  (
    .S              ( wPR8_DONE    ),
    .I              (dacc8P_done   ),
    .O              (wacc8P_done   )
  );

  jit_blackbox     ACC_PR8 (
    .mO1_V_TREADY   (wffo8C_tready ),
    .mO1_V_TVALID   (dffo8C_tvalid ),
    .mO1_V_TDATA    (dffo8C_tdata  ),
    .sI1_V_TREADY   (dffo8A_tready ),
    .sI1_V_TVALID   (wffo8A_tvalid ),
    .sI1_V_TDATA    (wffo8A_tdata  ),
    .sI2_V_TREADY   (dffo8B_tready ),
    .sI2_V_TVALID   (wffo8B_tvalid ),
    .sI2_V_TDATA    (wffo8B_tdata  ),
//  .ap_start       (wacc8P_start  ),
//  .ap_done        (dacc8P_done   ),
//  .ap_idle        (wacc8P_idle   ),
//  .ap_ready       (wacc8P_ready  ),
    .arg1_V         (wacc8P_arg1_V ),
    .arg2_V         (wacc8P_arg2_V ),
    .arg3_V         (wacc8P_arg3_V ),
    //--------------(--------------),
    .ap_clk         (clk_100       ),
    .ap_rst_n       (rstn          )
  );

  prctrl         u_prc_PR8 (
    .ID             ( 4'h8         ),
    .PR_DONE        ( wPR8_DONE    ),
    //--------------(--------------),
    .PR_VALID       (ws50i_valid   ),
    .PR_DATA        (ws50i_data    ),
    .clk            (clk_100       ),
    .rstn           (rstn          )
  );
end
endgenerate // NUM_ACCs : 8
//==============================================================================
//    ______        _____ _____ ____ _   _
//   / ___\ \      / /_ _|_   _/ ___| | | |
//   \___ \\ \ /\ / / | |  | || |   | |_| |
//    ___) |\ V  V /  | |  | || |___|  _  |
//   |____/  \_/\_/  |___| |_| \____|_| |_|
//==============================================================================
  jit_switch #(NUM_ACCs) u_switch  (
    .sCMD_tready    (ws50i_rdy     ),
    .sCMD_tvalid    (ws50i_valid   ),
    .sCMD_tdata     (ws50i_data    ),
//  .mRSP_tready    (ws50o_rdy     ),
//  .mRSP_tvalid    (ws50o_valid   ),
//  .mRSP_tdata     (ws50o_data    ),
    .s1A_tready     (ws11i_rdy     ),
    .s1A_tvalid     (ws11i_valid   ),
    .s1A_tdata      (ws11i_data    ),
    .s1B_tready     (ws12i_rdy     ),
    .s1B_tvalid     (ws12i_valid   ),
    .s1B_tdata      (ws12i_data    ),
    .m1C_tready     (ws13o_rdy     ),
    .m1C_tvalid     (ws13o_valid   ),
    .m1C_tdata      (ws13o_data    ),
    .s1AT_tready    (ws14i_rdy     ),
    .s1AT_tvalid    (ws14i_valid   ),
    .s1AT_tdata     (ws14i_data    ),
    .s1BT_tready    (ws15i_rdy     ),
    .s1BT_tvalid    (ws15i_valid   ),
    .s1BT_tdata     (ws15i_data    ),
    .m1CT_tready    (ws16o_rdy     ),
    .m1CT_tvalid    (ws16o_valid   ),
    .m1CT_tdata     (ws16o_data    ),
    .s2A_tready     (ws21i_rdy     ),
    .s2A_tvalid     (ws21i_valid   ),
    .s2A_tdata      (ws21i_data    ),
    .s2B_tready     (ws22i_rdy     ),
    .s2B_tvalid     (ws22i_valid   ),
    .s2B_tdata      (ws22i_data    ),
    .m2C_tready     (ws23o_rdy     ),
    .m2C_tvalid     (ws23o_valid   ),
    .m2C_tdata      (ws23o_data    ),
    .s2AT_tready    (ws24i_rdy     ),
    .s2AT_tvalid    (ws24i_valid   ),
    .s2AT_tdata     (ws24i_data    ),
    .s2BT_tready    (ws25i_rdy     ),
    .s2BT_tvalid    (ws25i_valid   ),
    .s2BT_tdata     (ws25i_data    ),
    .m2CT_tready    (ws26o_rdy     ),
    .m2CT_tvalid    (ws26o_valid   ),
    .m2CT_tdata     (ws26o_data    ),
    .s3A_tready     (ws31i_rdy     ),
    .s3A_tvalid     (ws31i_valid   ),
    .s3A_tdata      (ws31i_data    ),
    .s3B_tready     (ws32i_rdy     ),
    .s3B_tvalid     (ws32i_valid   ),
    .s3B_tdata      (ws32i_data    ),
    .m3C_tready     (ws33o_rdy     ),
    .m3C_tvalid     (ws33o_valid   ),
    .m3C_tdata      (ws33o_data    ),
    .s3AT_tready    (ws34i_rdy     ),
    .s3AT_tvalid    (ws34i_valid   ),
    .s3AT_tdata     (ws34i_data    ),
    .s3BT_tready    (ws35i_rdy     ),
    .s3BT_tvalid    (ws35i_valid   ),
    .s3BT_tdata     (ws35i_data    ),
    .m3CT_tready    (ws36o_rdy     ),
    .m3CT_tvalid    (ws36o_valid   ),
    .m3CT_tdata     (ws36o_data    ),
    .s4A_tready     (ws41i_rdy     ),
    .s4A_tvalid     (ws41i_valid   ),
    .s4A_tdata      (ws41i_data    ),
    .s4B_tready     (ws42i_rdy     ),
    .s4B_tvalid     (ws42i_valid   ),
    .s4B_tdata      (ws42i_data    ),
    .m4C_tready     (ws43o_rdy     ),
    .m4C_tvalid     (ws43o_valid   ),
    .m4C_tdata      (ws43o_data    ),
    .s4AT_tready    (ws44i_rdy     ),
    .s4AT_tvalid    (ws44i_valid   ),
    .s4AT_tdata     (ws44i_data    ),
    .s4BT_tready    (ws45i_rdy     ),
    .s4BT_tvalid    (ws45i_valid   ),
    .s4BT_tdata     (ws45i_data    ),
    .m4CT_tready    (ws46o_rdy     ),
    .m4CT_tvalid    (ws46o_valid   ),
    .m4CT_tdata     (ws46o_data    ),
    .s5A_tready     (ws51i_rdy     ),
    .s5A_tvalid     (ws51i_valid   ),
    .s5A_tdata      (ws51i_data    ),
    .s5B_tready     (ws52i_rdy     ),
    .s5B_tvalid     (ws52i_valid   ),
    .s5B_tdata      (ws52i_data    ),
    .m5C_tready     (ws53o_rdy     ),
    .m5C_tvalid     (ws53o_valid   ),
    .m5C_tdata      (ws53o_data    ),
    .s5AT_tready    (ws54i_rdy     ),
    .s5AT_tvalid    (ws54i_valid   ),
    .s5AT_tdata     (ws54i_data    ),
    .s5BT_tready    (ws55i_rdy     ),
    .s5BT_tvalid    (ws55i_valid   ),
    .s5BT_tdata     (ws55i_data    ),
    .m5CT_tready    (ws56o_rdy     ),
    .m5CT_tvalid    (ws56o_valid   ),
    .m5CT_tdata     (ws56o_data    ),
    .s6A_tready     (ws61i_rdy     ),
    .s6A_tvalid     (ws61i_valid   ),
    .s6A_tdata      (ws61i_data    ),
    .s6B_tready     (ws62i_rdy     ),
    .s6B_tvalid     (ws62i_valid   ),
    .s6B_tdata      (ws62i_data    ),
    .m6C_tready     (ws63o_rdy     ),
    .m6C_tvalid     (ws63o_valid   ),
    .m6C_tdata      (ws63o_data    ),
    .s6AT_tready    (ws64i_rdy     ),
    .s6AT_tvalid    (ws64i_valid   ),
    .s6AT_tdata     (ws64i_data    ),
    .s6BT_tready    (ws65i_rdy     ),
    .s6BT_tvalid    (ws65i_valid   ),
    .s6BT_tdata     (ws65i_data    ),
    .m6CT_tready    (ws66o_rdy     ),
    .m6CT_tvalid    (ws66o_valid   ),
    .m6CT_tdata     (ws66o_data    ),
    .s7A_tready     (ws71i_rdy     ),
    .s7A_tvalid     (ws71i_valid   ),
    .s7A_tdata      (ws71i_data    ),
    .s7B_tready     (ws72i_rdy     ),
    .s7B_tvalid     (ws72i_valid   ),
    .s7B_tdata      (ws72i_data    ),
    .m7C_tready     (ws73o_rdy     ),
    .m7C_tvalid     (ws73o_valid   ),
    .m7C_tdata      (ws73o_data    ),
    .s7AT_tready    (ws74i_rdy     ),
    .s7AT_tvalid    (ws74i_valid   ),
    .s7AT_tdata     (ws74i_data    ),
    .s7BT_tready    (ws75i_rdy     ),
    .s7BT_tvalid    (ws75i_valid   ),
    .s7BT_tdata     (ws75i_data    ),
    .m7CT_tready    (ws76o_rdy     ),
    .m7CT_tvalid    (ws76o_valid   ),
    .m7CT_tdata     (ws76o_data    ),
    .s8A_tready     (ws81i_rdy     ),
    .s8A_tvalid     (ws81i_valid   ),
    .s8A_tdata      (ws81i_data    ),
    .s8B_tready     (ws82i_rdy     ),
    .s8B_tvalid     (ws82i_valid   ),
    .s8B_tdata      (ws82i_data    ),
    .m8C_tready     (ws83o_rdy     ),
    .m8C_tvalid     (ws83o_valid   ),
    .m8C_tdata      (ws83o_data    ),
    .s8AT_tready    (ws84i_rdy     ),
    .s8AT_tvalid    (ws84i_valid   ),
    .s8AT_tdata     (ws84i_data    ),
    .s8BT_tready    (ws85i_rdy     ),
    .s8BT_tvalid    (ws85i_valid   ),
    .s8BT_tdata     (ws85i_data    ),
    .m8CT_tready    (ws86o_rdy     ),
    .m8CT_tvalid    (ws86o_valid   ),
    .m8CT_tdata     (ws86o_data    ),
    .s1C_tready     (wacc1C_tready ),
    .s1C_tvalid     (wacc1C_tvalid ),
    .s1C_tdata      (wacc1C_tdata  ),
    .m1A_tready     (wacc1A_tready ),
    .m1A_tvalid     (wacc1A_tvalid ),
    .m1A_tdata      (wacc1A_tdata  ),
    .m1B_tready     (wacc1B_tready ),
    .m1B_tvalid     (wacc1B_tvalid ),
    .m1B_tdata      (wacc1B_tdata  ),
    .s2C_tready     (wacc2C_tready ),
    .s2C_tvalid     (wacc2C_tvalid ),
    .s2C_tdata      (wacc2C_tdata  ),
    .m2A_tready     (wacc2A_tready ),
    .m2A_tvalid     (wacc2A_tvalid ),
    .m2A_tdata      (wacc2A_tdata  ),
    .m2B_tready     (wacc2B_tready ),
    .m2B_tvalid     (wacc2B_tvalid ),
    .m2B_tdata      (wacc2B_tdata  ),
    .s3C_tready     (wacc3C_tready ),
    .s3C_tvalid     (wacc3C_tvalid ),
    .s3C_tdata      (wacc3C_tdata  ),
    .m3A_tready     (wacc3A_tready ),
    .m3A_tvalid     (wacc3A_tvalid ),
    .m3A_tdata      (wacc3A_tdata  ),
    .m3B_tready     (wacc3B_tready ),
    .m3B_tvalid     (wacc3B_tvalid ),
    .m3B_tdata      (wacc3B_tdata  ),
    .s4C_tready     (wacc4C_tready ),
    .s4C_tvalid     (wacc4C_tvalid ),
    .s4C_tdata      (wacc4C_tdata  ),
    .m4A_tready     (wacc4A_tready ),
    .m4A_tvalid     (wacc4A_tvalid ),
    .m4A_tdata      (wacc4A_tdata  ),
    .m4B_tready     (wacc4B_tready ),
    .m4B_tvalid     (wacc4B_tvalid ),
    .m4B_tdata      (wacc4B_tdata  ),
    .s5C_tready     (wacc5C_tready ),
    .s5C_tvalid     (wacc5C_tvalid ),
    .s5C_tdata      (wacc5C_tdata  ),
    .m5A_tready     (wacc5A_tready ),
    .m5A_tvalid     (wacc5A_tvalid ),
    .m5A_tdata      (wacc5A_tdata  ),
    .m5B_tready     (wacc5B_tready ),
    .m5B_tvalid     (wacc5B_tvalid ),
    .m5B_tdata      (wacc5B_tdata  ),
    .s6C_tready     (wacc6C_tready ),
    .s6C_tvalid     (wacc6C_tvalid ),
    .s6C_tdata      (wacc6C_tdata  ),
    .m6A_tready     (wacc6A_tready ),
    .m6A_tvalid     (wacc6A_tvalid ),
    .m6A_tdata      (wacc6A_tdata  ),
    .m6B_tready     (wacc6B_tready ),
    .m6B_tvalid     (wacc6B_tvalid ),
    .m6B_tdata      (wacc6B_tdata  ),
    .s7C_tready     (wacc7C_tready ),
    .s7C_tvalid     (wacc7C_tvalid ),
    .s7C_tdata      (wacc7C_tdata  ),
    .m7A_tready     (wacc7A_tready ),
    .m7A_tvalid     (wacc7A_tvalid ),
    .m7A_tdata      (wacc7A_tdata  ),
    .m7B_tready     (wacc7B_tready ),
    .m7B_tvalid     (wacc7B_tvalid ),
    .m7B_tdata      (wacc7B_tdata  ),
    .s8C_tready     (wacc8C_tready ),
    .s8C_tvalid     (wacc8C_tvalid ),
    .s8C_tdata      (wacc8C_tdata  ),
    .m8A_tready     (wacc8A_tready ),
    .m8A_tvalid     (wacc8A_tvalid ),
    .m8A_tdata      (wacc8A_tdata  ),
    .m8B_tready     (wacc8B_tready ),
    .m8B_tvalid     (wacc8B_tvalid ),
    .m8B_tdata      (wacc8B_tdata  ),
//  .ap1_start      (wacc1P_start  ),
//  .ap1_done       (wacc1P_done   ),
//  .ap1_idle       (wacc1P_idle   ),
//  .ap1_ready      (wacc1P_ready  ),
    .ap1_arg1_V     (wacc1P_arg1_V ),
    .ap1_arg2_V     (wacc1P_arg2_V ),
    .ap1_arg3_V     (wacc1P_arg3_V ),
//  .ap2_start      (wacc2P_start  ),
//  .ap2_done       (wacc2P_done   ),
//  .ap2_idle       (wacc2P_idle   ),
//  .ap2_ready      (wacc2P_ready  ),
    .ap2_arg1_V     (wacc2P_arg1_V ),
    .ap2_arg2_V     (wacc2P_arg2_V ),
    .ap2_arg3_V     (wacc2P_arg3_V ),
//  .ap3_start      (wacc3P_start  ),
//  .ap3_done       (wacc3P_done   ),
//  .ap3_idle       (wacc3P_idle   ),
//  .ap3_ready      (wacc3P_ready  ),
    .ap3_arg1_V     (wacc3P_arg1_V ),
    .ap3_arg2_V     (wacc3P_arg2_V ),
    .ap3_arg3_V     (wacc3P_arg3_V ),
//  .ap4_start      (wacc4P_start  ),
//  .ap4_done       (wacc4P_done   ),
//  .ap4_idle       (wacc4P_idle   ),
//  .ap4_ready      (wacc4P_ready  ),
    .ap4_arg1_V     (wacc4P_arg1_V ),
    .ap4_arg2_V     (wacc4P_arg2_V ),
    .ap4_arg3_V     (wacc4P_arg3_V ),
//  .ap5_start      (wacc5P_start  ),
//  .ap5_done       (wacc5P_done   ),
//  .ap5_idle       (wacc5P_idle   ),
//  .ap5_ready      (wacc5P_ready  ),
    .ap5_arg1_V     (wacc5P_arg1_V ),
    .ap5_arg2_V     (wacc5P_arg2_V ),
    .ap5_arg3_V     (wacc5P_arg3_V ),
//  .ap6_start      (wacc6P_start  ),
//  .ap6_done       (wacc6P_done   ),
//  .ap6_idle       (wacc6P_idle   ),
//  .ap6_ready      (wacc6P_ready  ),
    .ap6_arg1_V     (wacc6P_arg1_V ),
    .ap6_arg2_V     (wacc6P_arg2_V ),
    .ap6_arg3_V     (wacc6P_arg3_V ),
//  .ap7_start      (wacc7P_start  ),
//  .ap7_done       (wacc7P_done   ),
//  .ap7_idle       (wacc7P_idle   ),
//  .ap7_ready      (wacc7P_ready  ),
    .ap7_arg1_V     (wacc7P_arg1_V ),
    .ap7_arg2_V     (wacc7P_arg2_V ),
    .ap7_arg3_V     (wacc7P_arg3_V ),
//  .ap8_start      (wacc8P_start  ),
//  .ap8_done       (wacc8P_done   ),
//  .ap8_idle       (wacc8P_idle   ),
//  .ap8_ready      (wacc8P_ready  ),
    .ap8_arg1_V     (wacc8P_arg1_V ),
    .ap8_arg2_V     (wacc8P_arg2_V ),
    .ap8_arg3_V     (wacc8P_arg3_V ),
    .ACLK           (clk_100       ),
    .ARESETN        (rstn          )
  );
//==============================================================================
//    ___ ____    _    ____
//   |_ _/ ___|  / \  |  _ \
//    | | |     / _ \ | |_) |
//    | | |___ / ___ \|  __/
//   |___\____/_/   \_\_|
//==============================================================================
  jit_fifo     u_fifo_s100i (
    .s_axis_tready  ( s100i_rdy    ),
    .s_axis_tvalid  ( s100i_valid  ),
    .s_axis_tdata   ( s100i_data   ),
    .m_axis_tready  ( 1'b1         ),
    .m_axis_tvalid  (ws100i_valid  ),
    .m_axis_tdata   (ws100i_data   ),
    //--------------(--------------),
    .s_aclk         (clk           ),
    .m_aclk         (clk_100       ),
    .s_aresetn      (rstn          )
  );

  genvar  k;
  generate
  for(k = 0; k < 8; k = k + 1)
  begin
    assign swapped_idata[   k] = ws100i_data[   7-k];
    assign swapped_idata[ 8+k] = ws100i_data[ 8+7-k];
    assign swapped_idata[16+k] = ws100i_data[16+7-k];
    assign swapped_idata[24+k] = ws100i_data[24+7-k];
  end endgenerate

  ICAPE2 #(
    .ICAP_WIDTH("X32")
  )
  ICAPE2_inst       (
    .CLK            (clk_100       ),
    .O              (icap_output   ),
    .CSIB           (~ws100i_valid ),
    .RDWRB          (1'b0          ),
    .I              (swapped_idata )
  );

endmodule
