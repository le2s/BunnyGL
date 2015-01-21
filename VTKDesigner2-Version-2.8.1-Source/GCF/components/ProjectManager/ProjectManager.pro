######################################################################
# Generic Component Framework Library from VCreate Logic
#
# Provides a project manager component. Most useful in applications
# that save/load data from XML files. Different aspects of the
# project will be stored under different XML elements
######################################################################

include (../components.pri)

DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source ../Utils/interfaces

# Input
HEADERS  =  Project.h ProjectManagerComponent.h

SOURCES  =  Project.cpp ProjectManagerComponent.cpp

RESOURCES = ProjectManager.qrc
