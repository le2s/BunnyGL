######################################################################
# VTK Project Include
######################################################################

include(FindVTK.pri)

exists( $$VTK5_LIB_DIR/vtkCommon.lib ) {
    win32-msvc* {
        LIBS += $$VTK5_LIB_DIR/vtkCommon.lib \
                $$VTK5_LIB_DIR/vtkGraphics.lib \
                $$VTK5_LIB_DIR/vtkFiltering.lib \
                $$VTK5_LIB_DIR/vtkGenericFiltering.lib \
                $$VTK5_LIB_DIR/vtkRendering.lib \
                $$VTK5_LIB_DIR/vtkImaging.lib \
                $$VTK5_LIB_DIR/vtkIO.lib \
                $$VTK5_LIB_DIR/QVTK.lib \
                $$VTK5_LIB_DIR/vtkWidgets.lib \
                $$VTK5_LIB_DIR/vtkHybrid.lib \
                $$VTK5_LIB_DIR/vtkzlib.lib \
                $$VTK5_LIB_DIR/vtkfreetype.lib \
                $$VTK5_LIB_DIR/vtkexpat.lib \
                $$VTK5_LIB_DIR/vtkexoIIc.lib \
                $$VTK5_LIB_DIR/vtkDICOMParser.lib \
                $$VTK5_LIB_DIR/vtksys.lib \
                $$VTK5_LIB_DIR/QVTK.lib \
                $$VTK5_LIB_DIR/vtkVolumeRendering.lib
    }
}

exists( $$VTK5_LIB_DIR/libvtkCommon.dll.a ) {
    win32-g++ {
        LIBS += $$VTK5_LIB_DIR/libvtkCommon.dll.a \
                $$VTK5_LIB_DIR/libvtkGraphics.dll.a \
                $$VTK5_LIB_DIR/libvtkFiltering.dll.a \
                $$VTK5_LIB_DIR/libvtkGenericFiltering.dll.a \
                $$VTK5_LIB_DIR/libvtkRendering.dll.a \
                $$VTK5_LIB_DIR/libvtkImaging.dll.a \
                $$VTK5_LIB_DIR/libvtkIO.dll.a \
                $$VTK5_LIB_DIR/libQVTK.dll.a \
                $$VTK5_LIB_DIR/libvtkWidgets.dll.a \
                $$VTK5_LIB_DIR/libvtkHybrid.dll.a \
                $$VTK5_LIB_DIR/libvtkzlib.dll.a \
                $$VTK5_LIB_DIR/libvtkfreetype.dll.a \
                $$VTK5_LIB_DIR/libvtkexpat.dll.a \
                $$VTK5_LIB_DIR/libvtkexoIIc.dll.a \
                $$VTK5_LIB_DIR/libvtkDICOMParser.dll.a \
                $$VTK5_LIB_DIR/libvtksys.dll.a \
                $$VTK5_LIB_DIR/libQVTK.dll.a\
                $$VTK5_LIB_DIR/libvtkVolumeRendering.a
    }
}

exists( $$VTK5_LIB_DIR/libvtkCommon.so ) {
    unix {
        LIBS += -L$$VTK5_LIB_DIR \
                -lvtkCommon \
                -lvtkGraphics \
                -lvtkFiltering \
                -lvtkGenericFiltering \
                -lvtkRendering \
                -lvtkImaging \
                -lvtkIO \
                -lQVTK \
                -lvtkWidgets \
                -lvtkHybrid \
                -lvtkVolumeRendering
    }
}

exists( $$VTK5_INCLUDE_DIR/vtkVersion.h ) {
    INCLUDEPATH += $$VTK5_INCLUDE_DIR
    QT          += opengl
}


