######################################################################
# Generic Component Framework Library from VCreate Logic
#
# Provides a project manager component. Most useful in applications
# that save/load data from XML files. Different aspects of the
# project will be stored under different XML elements
######################################################################

include (../components.pri)

DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source

HEADERS         = MessageLogComponent.h
SOURCES         = MessageLogComponent.cpp
RESOURCES       = MessageLog.qrc
