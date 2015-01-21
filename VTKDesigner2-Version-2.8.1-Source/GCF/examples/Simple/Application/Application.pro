TEMPLATE = app
TARGET = SimpleApplication
DESTDIR = ../../../bin

# You can also use CONFIG += gcf_app, if you have set QMAKEFEATURES
# to point to GCF_SDK_ROOT/qmake_prf directory
include(../../../qmake_prf/gcf_app.prf)

SOURCES += \
    Main.cpp \
    ApplicationContainerComponent.cpp

HEADERS += \
    ApplicationContainerComponent.h

OTHER_FILES += \
    ApplicationContainer.xml \
    SimpleApplication.xml

RESOURCES += \
    Application.qrc
