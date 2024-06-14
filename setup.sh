#!/bin/bash

source /cvmfs/uboone.opensciencegrid.org/products/setup_uboone.sh 
setup uboonecode v08_00_00_81 -q e17:prof

export CONNECTEDNESS_BASE=$PWD
export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$PWD/Alg
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/lib
