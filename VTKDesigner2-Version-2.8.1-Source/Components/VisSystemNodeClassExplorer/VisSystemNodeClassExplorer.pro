######################################################################
# VTK Designer 2 - VisSystemNodeClassExplorer component
# 
# Standard node class explorer component for all drag-and-droppable
# nodes in VTK Designer
######################################################################

include(../../GCF/qmake_prf/gcf_component.prf)

DESTDIR = ../../bin
DEPENDPATH += . interfaces source
INCLUDEPATH += . interfaces source ../../Interfaces ../../Common

# Input
HEADERS += Source/CVisSystemNodeClassExplorer.h \
           Source/CVisSystemNodeClassExplorerComponent.h \
           Source/CVisSystemNodeClassExplorerTreeWidget.h
           
SOURCES += Source/CVisSystemNodeClassExplorer.cpp \
           Source/CVisSystemNodeClassExplorerComponent.cpp \
           Source/CVisSystemNodeClassExplorerTreeWidget.cpp
