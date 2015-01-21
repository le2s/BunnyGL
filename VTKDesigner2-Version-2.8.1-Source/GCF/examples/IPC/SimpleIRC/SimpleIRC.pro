TEMPLATE = app
TARGET = SimpleIRC
DESTDIR = ../../../bin

include(../../../qmake_prf/gcf_app.prf)
include(../../../qmake_prf/gcf_ipc.prf)

SOURCES += \
    Main.cpp \
    ContactList.cpp \
    SimpleIRC.cpp \
    IRCWindow.cpp

HEADERS += \
    ContactList.h \
    SimpleIRC.h \
    IRCWindow.h

OTHER_FILES += \
    SimpleIRC.xml

RESOURCES += \
    SimpleIRC.qrc

FORMS += \
    IRCWindow.ui
