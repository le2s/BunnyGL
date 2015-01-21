######################################################################
# Generic Component Framework Library from VCreate Logic
#
# Provides a multi-context, list based clipboard
######################################################################

include (../components.pri)

DEPENDPATH     += . interfaces source ../Utils/interfaces
INCLUDEPATH    += . interfaces source ../Utils/interfaces

HEADERS         = source/ClipboardComponent.h \
                  source/ClipboardWidget.h \
                  source/ClipboardModel.h

SOURCES         = source/ClipboardComponent.cpp \
                  source/ClipboardWidget.cpp \
                  source/ClipboardModel.cpp

FORMS           = source/ClipboardWidget.ui

