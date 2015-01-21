######################################################################
# TemplateManager from VCreate Logic Pvt Ltd
######################################################################

include(../../GCF/qmake_prf/gcf_component.prf)

DESTDIR = ../../bin
DEPENDPATH += . interfaces source
INCLUDEPATH += . interfaces source ../../Interfaces ../../Common

HEADERS         += source/TemplateManagerComponent.h
SOURCES         += source/TemplateManagerComponent.cpp
