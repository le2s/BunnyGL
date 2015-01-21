######################################################################
# Generic Component Framework Library from VCreate Logic
# 
# Common configuration settings for most components
######################################################################

include(../GCF/config.pri)

TEMPLATE        = lib
CONFIG         += dll 
LIBS           += $${GCF_LIBS}
DEPENDPATH     += . source $${PROJECT_BASE}/Interfaces $${PROJECT_BASE}/Common
INCLUDEPATH    += . source $${PROJECT_BASE}/Interfaces $${PROJECT_BASE}/Common

# Interfaces to other standard components
INCLUDEPATH += $${GCF_BASE}/components/PropertyEditor/interfaces
INCLUDEPATH += $${GCF_BASE}/components/Utils/interfaces
INCLUDEPATH += $${GCF_BASE}/components/ScriptEditor/interfaces
INCLUDEPATH += $${GCF_BASE}/components/ProjectManager/interfaces
INCLUDEPATH += $${GCF_BASE}/components/MainWindow/source

