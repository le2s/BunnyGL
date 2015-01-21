######################################################################
# VTK Designer 2 - VTKFullscreen component
# 
# Allows viewing of 3D output in full screen.
######################################################################

include(../../GCF/qmake_prf/gcf_component.prf)
include(../../VTK.pri)

DESTDIR = ../../bin
DEPENDPATH += . interfaces source
INCLUDEPATH += . interfaces source ../../Interfaces ../../Common

# Input
HEADERS += source/CFullscreenRenderWindow.h source/CVTKFullscreenComponent.h
FORMS += source/CFullscreenRenderWindowToolbar.ui
SOURCES += source/CFullscreenRenderWindow.cpp source/CVTKFullscreenComponent.cpp
RESOURCES += VTKFullscreen.qrc
