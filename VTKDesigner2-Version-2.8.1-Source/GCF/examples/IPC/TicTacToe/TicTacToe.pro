TEMPLATE = app
TARGET = TicTacToe
DESTDIR = ../../../bin

include(../../../qmake_prf/gcf_app.prf)
include(../../../qmake_prf/gcf_ipc.prf)

SOURCES += \
    Main.cpp \
    TicTacToeWidget.cpp \
    TicTacToeComponent.cpp

HEADERS += \
    TicTacToeWidget.h \
    TicTacToeComponent.h

OTHER_FILES += \
    TicTacToeComponent.xml

RESOURCES += \
    TicTacToe.qrc
