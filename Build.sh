#!/bin/bash

g++ `${ROOTSYS}/bin/root-config --cflags` -c -fPIC $CONNECTEDNESS_BASE/Alg/ClusterBuilder.cxx  -o $CONNECTEDNESS_BASE/lib/ClusterBuilder.o
g++ -shared `${ROOTSYS}/bin/root-config --libs` $CONNECTEDNESS_BASE/lib/ClusterBuilder.o -o $CONNECTEDNESS_BASE/lib/libClusterBuilder.so

