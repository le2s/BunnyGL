TEMPLATE = app
TARGET = StdMainWindowExample
DESTDIR = ../../bin

# You can also use CONFIG += gcf_app, if you have set QMAKEFEATURES
# to point to GCF_SDK_ROOT/qmake_prf directory
include(../../qmake_prf/gcf_app.prf)
include(../../qmake_prf/gcf_mainwindow.prf)

SOURCES += \
    Main.cpp

OTHER_FILES += \
    StdMainWindowExample.xml \
    MainWindowComponent.xml \
    PropertyEditorComponent.xml \
    ScriptEditorComponent.xml
