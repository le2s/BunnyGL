######################################################################
# ScientificCalculator from My Organization
######################################################################

include(../../GCF/qmake_prf/gcf_component.prf)

DESTDIR = ../../bin
DEPENDPATH += . interfaces source
INCLUDEPATH += . interfaces source ../../Interfaces ../../Common

HEADERS         += source/ScientificCalculatorComponent.h \
                   source/CalculatorWidget.h
                   
SOURCES         += source/ScientificCalculatorComponent.cpp \
                   source/CalculatorWidget.cpp
                   
FORMS           += source/CalculatorWidget.ui

QT              += script
