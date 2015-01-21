TEMPLATE = lib
TARGET = Component2
DESTDIR = ../../../bin

# You can also use CONFIG += gcf_component, if you have set QMAKEFEATURES
# to point to GCF_SDK_ROOT/qmake_prf directory
include(../../../qmake_prf/gcf_component.prf)

HEADERS += \
    Component2.h

SOURCES += \
    Component2.cpp

OTHER_FILES += \
    Component2.xml

RESOURCES += \
    Component2.qrc
