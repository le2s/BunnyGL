######################################################################
# VTK Designer 2 - DataFileLoader component
# 
# Data file loader component
######################################################################

include(../../GCF/qmake_prf/gcf_component.prf)
include(../../VTK.pri)

DESTDIR = ../../bin
DEPENDPATH += . interfaces source
INCLUDEPATH += . interfaces source ../../Interfaces ../../Common

# Input
HEADERS += interfaces/IDataFileLoader.h \
           interfaces/IDataFileLoaderRegistry.h \
           source/CDataFileLoaderComponent.h \
           source/CDICOMFileLoader.h \
           source/CImageFileLoader.h \
           source/CPDBFileLoader.h \
           source/CPlot3DFileLoader.h \
           source/CVTKDataFileLoader.h \
           source/CTerrainImageFileLoader.h \
           source/CVRMLFileLoader.h \
    source/CSTLFileLoader.h
           
FORMS += source/CDFLLoadOptionsDialog.ui source/CDICOMFileLoadOptionsDialog.ui

SOURCES += source/CDataFileLoaderComponent.cpp \
           source/CDICOMFileLoader.cpp \
           source/CImageFileLoader.cpp \
           source/CPDBFileLoader.cpp \
           source/CPlot3DFileLoader.cpp \
           source/CVTKDataFileLoader.cpp \
           source/CTerrainImageFileLoader.cpp \
           source/CVRMLFileLoader.cpp \
    source/CSTLFileLoader.cpp

RESOURCES += DataFileLoader.qrc
