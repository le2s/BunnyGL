TEMPLATE = lib
TARGET = DICOMService
DESTDIR = ../../../bin

include(../../../qmake_prf/gcf_component.prf)
include(VTK.pri)

HEADERS         = DICOMService.h \
                  DICOMImageSeries.h \
                  DICOMImageSeriesMPR.h \
                  PatientList.h \
                  DICOMImageSeriesVolume.h

SOURCES         = DICOMService.cpp \
                  DICOMImageSeries.cpp \
                  DICOMImageSeriesMPR.cpp \
                  PatientList.cpp \
                  DICOMImageSeriesVolume.cpp

RESOURCES       = DICOMService.qrc

OTHER_FILES += \
    DICOMService.xml
