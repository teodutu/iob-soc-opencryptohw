# ----------------------------------------------------------------------------
#     _____
#    /     \
#   /____   \____
#  / \===\   \==/
# /___\===\___\/  AVNET Design Resource Center
#      \======/         www.em.avnet.com/drc
#       \====/    
# ----------------------------------------------------------------------------
#  
#  Disclaimer:
#     Avnet, Inc. makes no warranty for the use of this code or design.
#     This code is provided  "As Is". Avnet, Inc assumes no responsibility for
#     any errors, which may appear in this code, nor does it make a commitment
#     to update the information contained herein. Avnet, Inc specifically
#     disclaims any implied warranties of fitness for a particular purpose.
#                      Copyright(c) 2009 Avnet, Inc.
#                              All rights reserved.
# 
# ----------------------------------------------------------------------------

## DDR4 Interface
set_property PACKAGE_PIN AA25 [get_ports {c0_ddr4_ck_t[0]}]
set_property PACKAGE_PIN AB25 [get_ports {c0_ddr4_ck_c[0]}]
set_property IOSTANDARD DIFF_SSTL12_DCI [ get_ports {c0_ddr4_ck_t[0]} ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports {c0_ddr4_ck_t[0]} ]
set_property IOSTANDARD DIFF_SSTL12_DCI [ get_ports {c0_ddr4_ck_c[0]} ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports {c0_ddr4_ck_c[0]} ]

set_property PACKAGE_PIN AA24 [get_ports {c0_ddr4_adr[0]}]
set_property PACKAGE_PIN AB24 [get_ports {c0_ddr4_adr[1]}]
set_property PACKAGE_PIN AB26 [get_ports {c0_ddr4_adr[2]}]
set_property PACKAGE_PIN AC26 [get_ports {c0_ddr4_adr[3]}]
set_property PACKAGE_PIN AA22 [get_ports {c0_ddr4_adr[4]}]
set_property PACKAGE_PIN AB22 [get_ports {c0_ddr4_adr[5]}]
set_property PACKAGE_PIN Y23 [get_ports {c0_ddr4_adr[6]}]
set_property PACKAGE_PIN AA23 [get_ports {c0_ddr4_adr[7]}]
set_property PACKAGE_PIN AC23 [get_ports {c0_ddr4_adr[8]}]
set_property PACKAGE_PIN AC24 [get_ports {c0_ddr4_adr[9]}]
set_property PACKAGE_PIN W23 [get_ports {c0_ddr4_adr[10]}]
set_property PACKAGE_PIN W24 [get_ports {c0_ddr4_adr[11]}]
set_property PACKAGE_PIN W25 [get_ports {c0_ddr4_adr[12]}]
set_property PACKAGE_PIN W26 [get_ports {c0_ddr4_adr[13]}]

set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[0]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[1]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[2]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[3]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[4]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[5]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[6]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[7]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[8]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[9]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[10]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[11]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[12]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[13]}]

set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[0]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[1]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[2]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[3]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[4]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[5]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[6]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[7]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[8]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[9]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[10]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[11]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[12]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[13]}]

set_property PACKAGE_PIN F22 [get_ports {c0_ddr4_dqs_t[3]}]
set_property PACKAGE_PIN F23 [get_ports {c0_ddr4_dqs_c[3]}]
set_property PACKAGE_PIN K26 [get_ports {c0_ddr4_dqs_t[2]}]
set_property PACKAGE_PIN J26 [get_ports {c0_ddr4_dqs_c[2]}]
set_property PACKAGE_PIN T19 [get_ports {c0_ddr4_dqs_t[1]}]
set_property PACKAGE_PIN T20 [get_ports {c0_ddr4_dqs_c[1]}]
set_property PACKAGE_PIN R22 [get_ports {c0_ddr4_dqs_t[0]}]
set_property PACKAGE_PIN R23 [get_ports {c0_ddr4_dqs_c[0]}]

set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dqs_t[3]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dqs_c[3]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dqs_t[2]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dqs_c[2]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dqs_t[1]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dqs_c[1]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dqs_t[0]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dqs_c[0]}]

set_property IOSTANDARD DIFF_POD12_DCI [get_ports {c0_ddr4_dqs_t[3]}]
set_property IOSTANDARD DIFF_POD12_DCI [get_ports {c0_ddr4_dqs_c[3]}]
set_property IOSTANDARD DIFF_POD12_DCI [get_ports {c0_ddr4_dqs_t[2]}]
set_property IOSTANDARD DIFF_POD12_DCI [get_ports {c0_ddr4_dqs_c[2]}]
set_property IOSTANDARD DIFF_POD12_DCI [get_ports {c0_ddr4_dqs_t[1]}]
set_property IOSTANDARD DIFF_POD12_DCI [get_ports {c0_ddr4_dqs_c[1]}]
set_property IOSTANDARD DIFF_POD12_DCI [get_ports {c0_ddr4_dqs_t[0]}]
set_property IOSTANDARD DIFF_POD12_DCI [get_ports {c0_ddr4_dqs_c[0]}]

set_property PACKAGE_PIN E25 [get_ports {c0_ddr4_dm_dbi_n[3]}]
set_property PACKAGE_PIN N23 [get_ports {c0_ddr4_dm_dbi_n[2]}]
set_property PACKAGE_PIN R18 [get_ports {c0_ddr4_dm_dbi_n[1]}]
set_property PACKAGE_PIN T23 [get_ports {c0_ddr4_dm_dbi_n[0]}]

set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dm_dbi_n[3]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dm_dbi_n[2]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dm_dbi_n[1]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dm_dbi_n[0]}]

set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dm_dbi_n[3]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dm_dbi_n[2]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dm_dbi_n[1]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dm_dbi_n[0]}]

set_property PACKAGE_PIN G24 [get_ports {c0_ddr4_dq[31]}]
set_property PACKAGE_PIN H24 [get_ports {c0_ddr4_dq[30]}]
set_property PACKAGE_PIN J25 [get_ports {c0_ddr4_dq[29]}]
set_property PACKAGE_PIN J24 [get_ports {c0_ddr4_dq[28]}]
set_property PACKAGE_PIN F25 [get_ports {c0_ddr4_dq[27]}]
set_property PACKAGE_PIN G25 [get_ports {c0_ddr4_dq[26]}]
set_property PACKAGE_PIN G26 [get_ports {c0_ddr4_dq[25]}]
set_property PACKAGE_PIN H26 [get_ports {c0_ddr4_dq[24]}]
set_property PACKAGE_PIN L25 [get_ports {c0_ddr4_dq[23]}]
set_property PACKAGE_PIN M25 [get_ports {c0_ddr4_dq[22]}]
set_property PACKAGE_PIN M22 [get_ports {c0_ddr4_dq[21]}]
set_property PACKAGE_PIN N22 [get_ports {c0_ddr4_dq[20]}]
set_property PACKAGE_PIN L24 [get_ports {c0_ddr4_dq[19]}]
set_property PACKAGE_PIN M24 [get_ports {c0_ddr4_dq[18]}]
set_property PACKAGE_PIN M26 [get_ports {c0_ddr4_dq[17]}]
set_property PACKAGE_PIN N26 [get_ports {c0_ddr4_dq[16]}]
set_property PACKAGE_PIN U21 [get_ports {c0_ddr4_dq[15]}]
set_property PACKAGE_PIN U20 [get_ports {c0_ddr4_dq[14]}]
set_property PACKAGE_PIN R20 [get_ports {c0_ddr4_dq[13]}]
set_property PACKAGE_PIN P20 [get_ports {c0_ddr4_dq[12]}]
set_property PACKAGE_PIN P19 [get_ports {c0_ddr4_dq[11]}]
set_property PACKAGE_PIN P18 [get_ports {c0_ddr4_dq[10]}]
set_property PACKAGE_PIN R21 [get_ports {c0_ddr4_dq[9]}]
set_property PACKAGE_PIN P21 [get_ports {c0_ddr4_dq[8]}]
set_property PACKAGE_PIN R25 [get_ports {c0_ddr4_dq[7]}]
set_property PACKAGE_PIN P25 [get_ports {c0_ddr4_dq[6]}]
set_property PACKAGE_PIN P24 [get_ports {c0_ddr4_dq[5]}]
set_property PACKAGE_PIN P23 [get_ports {c0_ddr4_dq[4]}]
set_property PACKAGE_PIN R26 [get_ports {c0_ddr4_dq[3]}]
set_property PACKAGE_PIN P26 [get_ports {c0_ddr4_dq[2]}]
set_property PACKAGE_PIN U22 [get_ports {c0_ddr4_dq[1]}]
set_property PACKAGE_PIN T22 [get_ports {c0_ddr4_dq[0]}]

set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[31]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[30]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[29]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[28]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[27]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[26]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[25]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[24]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[23]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[22]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[21]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[20]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[19]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[18]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[17]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[16]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[15]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[14]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[13]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[12]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[11]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[10]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[9]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[8]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[7]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[6]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[5]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[4]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[3]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[2]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[1]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_dq[0]}]

set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[31]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[30]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[29]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[28]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[27]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[26]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[25]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[24]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[23]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[22]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[21]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[20]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[19]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[18]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[17]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[16]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[15]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[14]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[13]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[12]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[11]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[10]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[9]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[8]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[7]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[6]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[5]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[4]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[3]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[2]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[1]}]
set_property IOSTANDARD POD12_DCI [get_ports {c0_ddr4_dq[0]}]

## WE_N Signal
set_property PACKAGE_PIN Y25 [get_ports {c0_ddr4_adr[14]}]
## CAS_N Signal
set_property PACKAGE_PIN Y26 [get_ports {c0_ddr4_adr[15]}]
## RAS_N Signal
set_property PACKAGE_PIN U26 [get_ports {c0_ddr4_adr[16]}]

set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[14]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[15]}]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [get_ports {c0_ddr4_adr[16]}]

set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[14]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[15]}]
set_property IOSTANDARD SSTL12_DCI [get_ports {c0_ddr4_adr[16]}]

set_property PACKAGE_PIN T24 [get_ports {c0_ddr4_act_n}]
set_property IOSTANDARD SSTL12_DCI [ get_ports {c0_ddr4_act_n} ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports {c0_ddr4_act_n} ]

set_property PACKAGE_PIN T25 [get_ports c0_ddr4_reset_n]
set_property IOSTANDARD LVCMOS12 [ get_ports {c0_ddr4_reset_n} ]
set_property DRIVE 8 [get_ports c0_ddr4_reset_n]

set_property PACKAGE_PIN U25 [get_ports {c0_ddr4_odt[0]}]
set_property IOSTANDARD SSTL12_DCI [ get_ports {c0_ddr4_odt[0]} ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports {c0_ddr4_odt[0]} ]

set_property PACKAGE_PIN V22 [get_ports {c0_ddr4_cs_n[0]}]
set_property IOSTANDARD SSTL12_DCI [ get_ports {c0_ddr4_cs_n[0]} ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports {c0_ddr4_cs_n[0]} ]

set_property PACKAGE_PIN V23 [get_ports {c0_ddr4_cke[0]}]
set_property IOSTANDARD SSTL12_DCI [ get_ports {c0_ddr4_cke[0]} ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports {c0_ddr4_cke[0]} ]

set_property PACKAGE_PIN V26 [get_ports {c0_ddr4_ba[0]}]
set_property IOSTANDARD SSTL12_DCI [ get_ports {c0_ddr4_ba[0]} ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports {c0_ddr4_ba[0]} ]

set_property PACKAGE_PIN U24 [get_ports {c0_ddr4_ba[1]}]
set_property IOSTANDARD SSTL12_DCI [ get_ports {c0_ddr4_ba[1]} ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports {c0_ddr4_ba[1]} ]

set_property PACKAGE_PIN V24 [get_ports {c0_ddr4_bg[0]}]
set_property IOSTANDARD SSTL12_DCI [ get_ports {c0_ddr4_bg[0]} ]
set_property OUTPUT_IMPEDANCE RDRV_40_40 [ get_ports {c0_ddr4_bg[0]} ]
