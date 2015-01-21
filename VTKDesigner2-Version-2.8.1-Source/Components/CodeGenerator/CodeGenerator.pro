######################################################################
# CodeGenerator from VCreate Logic Private Limited
######################################################################

include(../../GCF/qmake_prf/gcf_component.prf)
include(../../VTK.pri)

DESTDIR = ../../bin
DEPENDPATH += . interfaces source
INCLUDEPATH += . interfaces source ../../Interfaces ../../Common \
               ../../Components/VTKSystem/Interfaces

HEADERS         += source/CodeGeneratorComponent.h \
                   source/CppCodeGenerator.h \
                   source/CppCodeGenerator_p.h
                   
SOURCES         += source/CodeGeneratorComponent.cpp \
                   source/CppCodeGenerator.cpp \
                   source/CppCodeGenerator_p.cpp

RESOURCES       += CodeGenerator.qrc
