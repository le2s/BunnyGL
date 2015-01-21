#!/bin/bash
export LD_LIBRARY_PATH=`pwd`/VTK5/bin:`pwd`/bin:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=/usr/local/vtk/release/lib/vtk-5.10:`pwd`/bin:`pwd`/lib:$DYLD_LIBRARY_PATH
`pwd`/bin/VTKDesigner2




