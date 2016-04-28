ipx::add_bus_interface $PNAME [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property enablement_dependency {$Num_Accs<=2} [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
ipx::add_port_map TREADY [ipx::get_bus_interfaces s12i -of_objects [ipx::current_core]]
set_property physical_name s12i_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces s12i -of_objects [ipx::current_core]]]
ipx::add_port_map TVALID [ipx::get_bus_interfaces s12i -of_objects [ipx::current_core]]
set_property physical_name s12i_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces s12i -of_objects [ipx::current_core]]]
ipx::add_port_map TDATA [ipx::get_bus_interfaces s12i -of_objects [ipx::current_core]]
set_property physical_name s12i_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces s12i -of_objects [ipx::current_core]]]

set PNAME s13o
set NUM     3
ipx::add_bus_interface $PNAME [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property interface_mode master [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property enablement_dependency {$NUM_ACCs<=[expr $NUM]} [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
# ==================================================================================================
set PNAME s21i
set NUM    2
ipx::add_bus_interface $PNAME [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property enablement_dependency \$NUM_ACCs<=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

set PNAME s22i
set NUM    2
ipx::add_bus_interface $PNAME [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property enablement_dependency \$NUM_ACCs<=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

set PNAME s23o
set NUM    2
ipx::add_bus_interface $PNAME [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property interface_mode master [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property enablement_dependency \$NUM_ACCs<=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

# ==================================================================================================
set NUM 50
set PNAME s$NUM\i
set ACLK_NAME $PNAME
ipx::add_bus_interface $PNAME [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
# set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

set NUM 100
set PNAME s$NUM\i
set ACLK_NAME $ACLK_NAME:$PNAME
ipx::add_bus_interface $PNAME [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
# set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
# ==================================================================================================
set PORT_NAME_LIST {1 2 3 4 5 6 7 8}
#####################################
set LIST_LEN [llength $PORT_NAME_LIST]
for {set ii 0} {$ii < $LIST_LEN} {incr ii} \
{
  set NUM [lindex $PORT_NAME_LIST $ii]
  # ------------
  set PNAME s$NUM[expr 1]i
  set ACLK_NAME $ACLK_NAME:$PNAME
  ipx::add_bus_interface $PNAME [ipx::current_core]
  set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

  set PNAME s$NUM[expr 2]i
  set ACLK_NAME $ACLK_NAME:$PNAME
  ipx::add_bus_interface $PNAME [ipx::current_core]
  set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

  set PNAME s$NUM[expr 3]o
  set ACLK_NAME $ACLK_NAME:$PNAME
  ipx::add_bus_interface $PNAME [ipx::current_core]
  set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property interface_mode master [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_rdy [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_valid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_data [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
}

set_property value ACTIVE_HIGH [ipx::get_bus_parameters POLARITY -of_objects [ipx::get_bus_interfaces rst -of_objects [ipx::current_core]]]
set_property value $ACLK_NAME [ipx::get_bus_parameters ASSOCIATED_BUSIF -of_objects [ipx::get_bus_interfaces clk -of_objects [ipx::current_core]]]
==================================================================================================



# ==================================================================================================
set PNAME sCMD
set ACLK_NAME $PNAME
ipx::add_bus_interface $PNAME [ipx::current_core]
set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_tready [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_tvalid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
set_property physical_name $PNAME\_tdata [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
# ==================================================================================================
set PORT_NAME_LIST {1 2 3 4 5 6 7 8}
#####################################
set LIST_LEN [llength $PORT_NAME_LIST]
for {set ii 0} {$ii < $LIST_LEN} {incr ii} \
{
  set NUM [lindex $PORT_NAME_LIST $ii]
  # ------------
  set PNAME s$NUM\A
  set ACLK_NAME $ACLK_NAME:$PNAME
  ipx::add_bus_interface $PNAME [ipx::current_core]
  set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tready [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tvalid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tdata [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

  set PNAME s$NUM\B
  set ACLK_NAME $ACLK_NAME:$PNAME
  ipx::add_bus_interface $PNAME [ipx::current_core]
  set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tready [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tvalid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tdata [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

  set PNAME m$NUM\C
  set ACLK_NAME $ACLK_NAME:$PNAME
  ipx::add_bus_interface $PNAME [ipx::current_core]
  set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property interface_mode master [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tready [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tvalid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tdata [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
}

# ==================================================================================================
set PORT_NAME_LIST {1 2 3 4 5 6 7 8}
#####################################
set LIST_LEN [llength $PORT_NAME_LIST]
for {set ii 0} {$ii < $LIST_LEN} {incr ii} \
{
  set NUM [lindex $PORT_NAME_LIST $ii]
  # ------------
  set PNAME s$NUM\C
  set ACLK_NAME $ACLK_NAME:$PNAME
  ipx::add_bus_interface $PNAME [ipx::current_core]
  set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tready [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tvalid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tdata [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

  set PNAME m$NUM\A
  set ACLK_NAME $ACLK_NAME:$PNAME
  ipx::add_bus_interface $PNAME [ipx::current_core]
  set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property interface_mode master [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tready [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tvalid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tdata [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]

  set PNAME m$NUM\B
  set ACLK_NAME $ACLK_NAME:$PNAME
  ipx::add_bus_interface $PNAME [ipx::current_core]
  set_property abstraction_type_vlnv xilinx.com:interface:axis_rtl:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property bus_type_vlnv xilinx.com:interface:axis:1.0 [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property interface_mode master [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  ipx::add_port_map TREADY [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tready [ipx::get_port_maps TREADY -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TVALID [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tvalid [ipx::get_port_maps TVALID -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
  ipx::add_port_map TDATA [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]
  set_property physical_name $PNAME\_tdata [ipx::get_port_maps TDATA -of_objects [ipx::get_bus_interfaces $PNAME -of_objects [ipx::current_core]]]
}

set_property value ACTIVE_HIGH [ipx::get_bus_parameters POLARITY -of_objects [ipx::get_bus_interfaces rst -of_objects [ipx::current_core]]]
set_property value $ACLK_NAME [ipx::get_bus_parameters ASSOCIATED_BUSIF -of_objects [ipx::get_bus_interfaces ACLK -of_objects [ipx::current_core]]]

set PORT_NAME_LIST {1 2 3 4 5 6 7 8}
#####################################
set LIST_LEN [llength $PORT_NAME_LIST]
for {set ii 0} {$ii < $LIST_LEN} {incr ii} \
{
  set NUM [lindex $PORT_NAME_LIST $ii]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_ports ap$NUM\_arg1_V -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_ports ap$NUM\_arg2_V -of_objects [ipx::current_core]]
  set_property enablement_dependency \$NUM_ACCs>=[expr $NUM] [ipx::get_ports ap$NUM\_arg3_V -of_objects [ipx::current_core]]
}


