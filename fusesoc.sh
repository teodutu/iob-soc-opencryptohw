#!/usr/bin/env bash

CORE=iobundle:opencryptohw:0.0.1
case "$1" in
    "sim-setup")
    fusesoc run --target=sim --setup $CORE
    ;;
    "sim-build")
    fusesoc run --target=sim --setup --build $CORE
    ;;
    "sim")
    fusesoc run --target=sim $CORE
    ;;
    "fpga-setup")
    fusesoc run --target=fpga --setup $CORE
    ;;
    "fpga-build")
    source /opt/Xilinx/Vivado/2020.2/settings64.sh
    fusesoc run --target=fpga --setup --build $CORE
    ;;
    "fpga")
    source /opt/Xilinx/Vivado/2020.2/settings64.sh
    fusesoc run --target=fpga $CORE
    ;;
    "clean")
    rm -rf build
    ;;
    *)
    echo "Supported arguments: "
    echo "          sim-setup sim-build sim"
    echo "          fpga-setup fpga-build fpga"
    echo "          clean"
    exit 1
    ;;
esac

