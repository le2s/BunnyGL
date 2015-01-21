######################################################################
# VRMLExporter from My Organization
######################################################################

include(../../GCF/qmake_prf/gcf_component.prf)
include(../../VTK.pri)

DESTDIR = ../../bin
DEPENDPATH += . interfaces source
INCLUDEPATH += . interfaces source ../../Interfaces ../../Common

HEADERS         += source/VRMLExporterComponent.h
SOURCES         += source/VRMLExporterComponent.cpp
