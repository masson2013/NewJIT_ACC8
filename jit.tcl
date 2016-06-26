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
#####################################################################################################

create_bd_cell -type ip -vlnv xilinx.com:ip:microblaze:* microblaze_0
apply_bd_automation -rule xilinx.com:bd_rule:microblaze -config {local_mem "64KB" ecc "None" cache "None" debug_module "Debug & UART" axi_periph "Enabled" axi_intc "0" clk "New Clocking Wizard (100 MHz)" }  [get_bd_cells microblaze_0]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "sys_diff_clock" }  [get_bd_intf_pins clk_wiz_1/CLK_IN1_D]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "reset" }  [get_bd_pins clk_wiz_1/reset]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "reset" }  [get_bd_pins rst_clk_wiz_1_100M/ext_reset_in]

create_bd_cell -type ip -vlnv xilinx.com:ip:axi_uartlite:* axi_uartlite_0
set_property location {2 663 703} [get_bd_cells axi_uartlite_0]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_uartlite_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:board -config {Board_Interface "rs232_uart" }  [get_bd_intf_pins axi_uartlite_0/UART]
set_property location {1 439 551} [get_bd_cells axi_uartlite_0]

create_bd_cell -type ip -vlnv xilinx.com:ip:axi_timer:* axi_timer_0
set_property -dict [list CONFIG.mode_64bit {1} CONFIG.enable_timer2 {0}] [get_bd_cells axi_timer_0]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_timer_0/S_AXI]
set_property location {6 2482 775} [get_bd_cells axi_timer_0]

set_property offset 0x40000000 [get_bd_addr_segs {microblaze_0/Data/SEG_mdm_1_Reg}]
set_property range 64K [get_bd_addr_segs {microblaze_0/Data/SEG_mdm_1_Reg}]

set_property offset 0x41000000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_uartlite_0_Reg}]
set_property range 64K [get_bd_addr_segs {microblaze_0/Data/SEG_axi_uartlite_0_Reg}]

set_property offset 0x42000000 [get_bd_addr_segs {microblaze_0/Data/SEG_axi_timer_0_Reg}]
set_property range 64K [get_bd_addr_segs {microblaze_0/Data/SEG_axi_timer_0_Reg}]


create_bd_cell -type ip -vlnv user.org:user:jit_switch:* jit_switch_0
set_property -dict [list CONFIG.NUM_ACCs {2}] [get_bd_cells jit_switch_0]
connect_bd_net [get_bd_pins jit_switch_0/ACLK] [get_bd_pins clk_wiz_1/clk_out1]
connect_bd_net [get_bd_pins jit_switch_0/ARESETN] [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn]

create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:* axi_gpio_0
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "Auto" }  [get_bd_intf_pins axi_gpio_0/S_AXI]




# set PORT_NAME_LIST {1 2 3 4 5 6 7 8}
set PORT_NAME_LIST {1 2}
#####################################################################################################
set LIST_LEN [llength $PORT_NAME_LIST]
for {set ii 0} {$ii < $LIST_LEN} {incr ii} \
{
  set NUM [lindex $PORT_NAME_LIST $ii]
  create_bd_cell -type ip -vlnv xilinx.com:hls:sgen:* sgen_$NUM\A
  create_bd_cell -type ip -vlnv xilinx.com:hls:sgen:* sgen_$NUM\B
  create_bd_cell -type ip -vlnv xilinx.com:hls:sget:* sget_$NUM\C

  create_bd_cell -type ip -vlnv xilinx.com:ip:fifo_generator:* fifo_$NUM\A
  set_property -dict [list CONFIG.INTERFACE_TYPE {AXI_STREAM} CONFIG.TDATA_NUM_BYTES {4} CONFIG.TUSER_WIDTH {0} CONFIG.TSTRB_WIDTH {4} CONFIG.TKEEP_WIDTH {4} CONFIG.FIFO_Implementation_wach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wach {15} CONFIG.Empty_Threshold_Assert_Value_wach {14} CONFIG.FIFO_Implementation_wrch {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wrch {15} CONFIG.Empty_Threshold_Assert_Value_wrch {14} CONFIG.FIFO_Implementation_rach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_rach {15} CONFIG.Empty_Threshold_Assert_Value_rach {14}] [get_bd_cells fifo_$NUM\A]
  create_bd_cell -type ip -vlnv xilinx.com:ip:fifo_generator:* fifo_$NUM\B
  set_property -dict [list CONFIG.INTERFACE_TYPE {AXI_STREAM} CONFIG.TDATA_NUM_BYTES {4} CONFIG.TUSER_WIDTH {0} CONFIG.TSTRB_WIDTH {4} CONFIG.TKEEP_WIDTH {4} CONFIG.FIFO_Implementation_wach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wach {15} CONFIG.Empty_Threshold_Assert_Value_wach {14} CONFIG.FIFO_Implementation_wrch {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wrch {15} CONFIG.Empty_Threshold_Assert_Value_wrch {14} CONFIG.FIFO_Implementation_rach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_rach {15} CONFIG.Empty_Threshold_Assert_Value_rach {14}] [get_bd_cells fifo_$NUM\B]
  create_bd_cell -type ip -vlnv xilinx.com:ip:fifo_generator:* fifo_$NUM\C
  set_property -dict [list CONFIG.INTERFACE_TYPE {AXI_STREAM} CONFIG.TDATA_NUM_BYTES {4} CONFIG.TUSER_WIDTH {0} CONFIG.TSTRB_WIDTH {4} CONFIG.TKEEP_WIDTH {4} CONFIG.FIFO_Implementation_wach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wach {15} CONFIG.Empty_Threshold_Assert_Value_wach {14} CONFIG.FIFO_Implementation_wrch {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wrch {15} CONFIG.Empty_Threshold_Assert_Value_wrch {14} CONFIG.FIFO_Implementation_rach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_rach {15} CONFIG.Empty_Threshold_Assert_Value_rach {14}] [get_bd_cells fifo_$NUM\C]

  create_bd_cell -type ip -vlnv xilinx.com:ip:fifo_generator:* fifo_acc_$NUM\A
  set_property -dict [list CONFIG.INTERFACE_TYPE {AXI_STREAM} CONFIG.TDATA_NUM_BYTES {4} CONFIG.TUSER_WIDTH {0} CONFIG.TSTRB_WIDTH {4} CONFIG.TKEEP_WIDTH {4} CONFIG.FIFO_Implementation_wach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wach {15} CONFIG.Empty_Threshold_Assert_Value_wach {14} CONFIG.FIFO_Implementation_wrch {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wrch {15} CONFIG.Empty_Threshold_Assert_Value_wrch {14} CONFIG.FIFO_Implementation_rach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_rach {15} CONFIG.Empty_Threshold_Assert_Value_rach {14}] [get_bd_cells fifo_acc_$NUM\A]
  create_bd_cell -type ip -vlnv xilinx.com:ip:fifo_generator:* fifo_acc_$NUM\B
  set_property -dict [list CONFIG.INTERFACE_TYPE {AXI_STREAM} CONFIG.TDATA_NUM_BYTES {4} CONFIG.TUSER_WIDTH {0} CONFIG.TSTRB_WIDTH {4} CONFIG.TKEEP_WIDTH {4} CONFIG.FIFO_Implementation_wach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wach {15} CONFIG.Empty_Threshold_Assert_Value_wach {14} CONFIG.FIFO_Implementation_wrch {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wrch {15} CONFIG.Empty_Threshold_Assert_Value_wrch {14} CONFIG.FIFO_Implementation_rach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_rach {15} CONFIG.Empty_Threshold_Assert_Value_rach {14}] [get_bd_cells fifo_acc_$NUM\B]
  create_bd_cell -type ip -vlnv xilinx.com:ip:fifo_generator:* fifo_acc_$NUM\C
  set_property -dict [list CONFIG.INTERFACE_TYPE {AXI_STREAM} CONFIG.TDATA_NUM_BYTES {4} CONFIG.TUSER_WIDTH {0} CONFIG.TSTRB_WIDTH {4} CONFIG.TKEEP_WIDTH {4} CONFIG.FIFO_Implementation_wach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wach {15} CONFIG.Empty_Threshold_Assert_Value_wach {14} CONFIG.FIFO_Implementation_wrch {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_wrch {15} CONFIG.Empty_Threshold_Assert_Value_wrch {14} CONFIG.FIFO_Implementation_rach {Common_Clock_Distributed_RAM} CONFIG.Full_Threshold_Assert_Value_rach {15} CONFIG.Empty_Threshold_Assert_Value_rach {14}] [get_bd_cells fifo_acc_$NUM\C]

  create_bd_cell -type ip -vlnv user.org:user:jit_blackbox:* jit_blackbox_$NUM

  connect_bd_intf_net [get_bd_intf_pins sgen_$NUM\A/OUT_V] [get_bd_intf_pins fifo_$NUM\A/S_AXIS]
  connect_bd_intf_net [get_bd_intf_pins sgen_$NUM\B/OUT_V] [get_bd_intf_pins fifo_$NUM\B/S_AXIS]
  connect_bd_intf_net [get_bd_intf_pins sget_$NUM\C/IN_V]  [get_bd_intf_pins fifo_$NUM\C/M_AXIS]

  connect_bd_intf_net [get_bd_intf_pins fifo_$NUM\A/M_AXIS] [get_bd_intf_pins jit_switch_0/s$NUM\A]
  connect_bd_intf_net [get_bd_intf_pins fifo_$NUM\B/M_AXIS] [get_bd_intf_pins jit_switch_0/s$NUM\B]
  connect_bd_intf_net [get_bd_intf_pins fifo_$NUM\C/S_AXIS] [get_bd_intf_pins jit_switch_0/m$NUM\C]

  connect_bd_intf_net [get_bd_intf_pins jit_switch_0/m$NUM\A] [get_bd_intf_pins fifo_acc_$NUM\A/S_AXIS]
  connect_bd_intf_net [get_bd_intf_pins fifo_acc_$NUM\A/M_AXIS] [get_bd_intf_pins jit_blackbox_$NUM/sI1_V]

  connect_bd_intf_net [get_bd_intf_pins jit_switch_0/m$NUM\B] [get_bd_intf_pins fifo_acc_$NUM\B/S_AXIS]
  connect_bd_intf_net [get_bd_intf_pins fifo_acc_$NUM\B/M_AXIS] [get_bd_intf_pins jit_blackbox_$NUM/sI2_V]

  connect_bd_intf_net [get_bd_intf_pins jit_blackbox_$NUM/mO1_V] [get_bd_intf_pins fifo_acc_$NUM\C/S_AXIS]
  connect_bd_intf_net [get_bd_intf_pins fifo_acc_$NUM\C/M_AXIS] [get_bd_intf_pins jit_switch_0/s$NUM\C]

  connect_bd_net [get_bd_pins jit_switch_0/ap$NUM\_arg1_V] [get_bd_pins jit_blackbox_$NUM/arg1_V]
  connect_bd_net [get_bd_pins jit_switch_0/ap$NUM\_arg2_V] [get_bd_pins jit_blackbox_$NUM/arg2_V]
  connect_bd_net [get_bd_pins jit_switch_0/ap$NUM\_arg3_V] [get_bd_pins jit_blackbox_$NUM/arg3_V]

  connect_bd_net [get_bd_pins axi_gpio_0/gpio_io_o] [get_bd_pins sgen_$NUM\A/arg1_V]
  connect_bd_net [get_bd_pins axi_gpio_0/gpio_io_o] [get_bd_pins sgen_$NUM\B/arg1_V]

  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins sgen_$NUM\A/ap_clk]
  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins sgen_$NUM\B/ap_clk]
  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins sget_$NUM\C/ap_clk]
  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins fifo_$NUM\A/s_aclk]
  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins fifo_$NUM\B/s_aclk]
  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins fifo_$NUM\C/s_aclk]
  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins fifo_acc_$NUM\A/s_aclk]
  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins fifo_acc_$NUM\B/s_aclk]
  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins fifo_acc_$NUM\C/s_aclk]
  connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins jit_blackbox_$NUM/ap_clk]

  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins sgen_$NUM\A/ap_rst_n]
  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins sgen_$NUM\B/ap_rst_n]
  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins sget_$NUM\C/ap_rst_n]
  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins fifo_$NUM\A/s_aresetn]
  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins fifo_$NUM\B/s_aresetn]
  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins fifo_$NUM\C/s_aresetn]
  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins fifo_acc_$NUM\A/s_aresetn]
  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins fifo_acc_$NUM\B/s_aresetn]
  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins fifo_acc_$NUM\C/s_aresetn]
  connect_bd_net [get_bd_pins rst_clk_wiz_1_100M/peripheral_aresetn] [get_bd_pins jit_blackbox_$NUM/ap_rst_n]
}
#####################################################################################################





























