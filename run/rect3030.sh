#! /bin/sh
if [ ! -f nextsim ]; then
    echo "Copy or link the nextsim executable here from the build directory"
fi
./nextsim --config-file rect3030.cfg
