TEMPLATE = lib
TARGET = FractalService
DESTDIR = ../../../bin

include(../../../qmake_prf/gcf_component.prf)

HEADERS += \
    MengerSquareFractal.h \
    FractalService.h \
    SierpinskiTriangleFractal.h

SOURCES += \
    MengerSquareFractal.cpp \
    FractalService.cpp \
    SierpinskiTriangleFractal.cpp

OTHER_FILES += \
    FractalService.xml

RESOURCES += \
    FractalService.qrc
