TEMPLATE = lib
TARGET = ExtendingGUIXMLComponent1
DESTDIR = ../../../bin

# You can also use CONFIG += gcf_component, if you have set QMAKEFEATURES
# to point to GCF_SDK_ROOT/qmake_prf directory
include(../../../qmake_prf/gcf_component.prf)

HEADERS += \
    Component1.h

SOURCES += \
    Component1.cpp

OTHER_FILES += \
    Component1.xml

RESOURCES += \
    Component1.qrc
