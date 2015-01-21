TEMPLATE = lib
TARGET = SimpleWebService
DESTDIR = ../../../bin

include(../../../qmake_prf/gcf_component.prf)

HEADERS += \
    SimpleWebService.h

SOURCES += \
    SimpleWebService.cpp

OTHER_FILES += \
    SimpleWebService.xml

RESOURCES += \
    SimpleWebService.qrc
