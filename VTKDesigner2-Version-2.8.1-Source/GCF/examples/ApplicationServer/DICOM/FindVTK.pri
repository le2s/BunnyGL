######################################################################
# VTK Project Include
######################################################################

# We expect that the VTK5_SDK_ROOT contains two directories
# Debug and Release. $$VTK5_DIR/Debug will (obviously)
# have debug versions of the VTK5 library, $$VTK5_DIR/Release
# will have release versions of the library

CONFIG(debug, debug|release) {
    VTK5_DIR=$$(VTK5_SDK_ROOT)/Debug
} else {
    VTK5_DIR=$$(VTK5_SDK_ROOT)/Release
}

win32-msvc* {
    VTK5_INCLUDE_DIR = $$VTK5_DIR/include/vtk-5.8/
    VTK5_LIB_DIR = $$VTK5_DIR/lib/vtk-5.8/
}

win32-g++ {
    VTK5_INCLUDE_DIR = $$VTK5_DIR/include/vtk-5.8/
    VTK5_LIB_DIR = $$VTK5_DIR/lib/vtk-5.8/
}

unix {
    exists( /usr/local/include/vtk-5.8/vtkVersion.h ) {
        VTK5_INCLUDE_DIR = /usr/local/include/vtk-5.8
        VTK5_LIB_DIR = /usr/local/lib/vtk-5.8
    } else {
        VTK5_INCLUDE_DIR = $$VTK5_DIR/include
        VTK5_LIB_DIR = $$VTK5_DIR/bin/
    }
}

!exists( $$VTK5_INCLUDE_DIR/vtkVersion.h ) {
    DEFINES += VTK5_NOT_FOUND
} else {
    DEFINES += VTK5_FOUND
}

