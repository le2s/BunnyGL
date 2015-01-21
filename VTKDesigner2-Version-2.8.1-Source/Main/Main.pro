######################################################################
# VTK Designer Main Window from VCreate Logic
######################################################################

TEMPLATE    = app
TARGET      = VTKDesigner2
DESTDIR     = ../bin
include(../GCF/qmake_prf/gcf_app.prf)
include(../GCF/qmake_prf/gcf_mainwindow.prf)

win32 {
    DOT_NET_STYLE_ROOT = ../3rdParty/qtdotnetstyle-2.3-opensource/src
    SOURCES += $$DOT_NET_STYLE_ROOT/qtdotnetstyle.cpp
    HEADERS += $$DOT_NET_STYLE_ROOT/qtdotnetstyle.h
    INCLUDEPATH += $$DOT_NET_STYLE_ROOT
}

CONFIG -= app_bundle

# Input
SOURCES += Main.cpp
RESOURCES += Main.qrc
CONFIG(debug, debug|release) {
    CONFIG += console
} else {
    CONFIG -= console
}
RC_FILE = icons/vtkd.rc

OTHER_FILES += \
    ../VTKDesigner2.nsi

