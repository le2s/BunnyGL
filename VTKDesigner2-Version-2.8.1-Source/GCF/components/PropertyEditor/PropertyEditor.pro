######################################################################
# Generic Component Framework Library from VCreate Logic
#
# Provides a robust, extensible and re-usable property editor component
######################################################################

include (../components.pri)

DEPENDPATH     += . interfaces source
INCLUDEPATH    += . interfaces source \
                  ../ScriptEditor/interfaces

# Input
HEADERS  = interfaces/IDefaultProperty.h \
           interfaces/IDefaultPropertyManager.h \
           interfaces/IEditableProperties.h \
           interfaces/IProperty.h \
           interfaces/IPropertyEditor.h \
           interfaces/IQObjectLoadSave.h \
           source/ColorPickerWidget.h \
           source/Property.h \
           source/PropertyEditor.h \
           source/PropertyEditorComponent.h \
           source/ValueEditorCreators.h \
           source/ValueEditorFactory.h \
    interfaces/IDefaultValueEditorFactory.h

FORMS    = source/ColorPickerWidget.ui

SOURCES  = source/ColorPickerWidget.cpp \
           source/Property.cpp \
           source/PropertyEditor.cpp \
           source/PropertyEditorComponent.cpp \
           source/ValueEditorCreators.cpp \
           source/ValueEditorFactory.cpp

RESOURCES = PropertyEditor.qrc

