/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkVolumeRayCastMIPFunction.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkVolumeRayCastMIPFunction.h"

DEFINE_VIS_NODE(CVtkVolumeRayCastMIPFunction, CVtkVolumeRayCastFunction)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVolumeRayCastMIPFunction");
    pDesc->setNodeClassDescription("vtkVolumeRayCastMIPFunction");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("MaximizeMethod", "SetMaximizeMethod", "GetMaximizeMethod", QStringList() << "int", QStringList(), "void", "int");
    // Uncomment and use the following code template to add input/output paths
    /*
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "PathName",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "PathType",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
    */
}

struct CVtkVolumeRayCastMIPFunctionData
{
    vtkVolumeRayCastMIPFunction *volumeRayCastMIPFunction;
};

CVtkVolumeRayCastMIPFunction::CVtkVolumeRayCastMIPFunction()
{
    CVtkVolumeRayCastMIPFunction::InitializeNodeDesc();
    d = new CVtkVolumeRayCastMIPFunctionData;

    d->volumeRayCastMIPFunction = 0;
    setVtkObject( vtkVolumeRayCastMIPFunction::New() );
}

CVtkVolumeRayCastMIPFunction::~CVtkVolumeRayCastMIPFunction()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->volumeRayCastMIPFunction->Delete();
    delete d;
}

void CVtkVolumeRayCastMIPFunction::setMaximizeMethod(CVtkVolumeRayCastMIPFunction::MaximizeMethod m)
{
    d->volumeRayCastMIPFunction->SetMaximizeMethod( int(m) );
}

CVtkVolumeRayCastMIPFunction::MaximizeMethod CVtkVolumeRayCastMIPFunction::maximizeMethod() const
{
    return CVtkVolumeRayCastMIPFunction::MaximizeMethod(
                d->volumeRayCastMIPFunction->GetMaximizeMethod()
            );
}

bool CVtkVolumeRayCastMIPFunction::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeRayCastFunction::hasInput(path);
}

bool CVtkVolumeRayCastMIPFunction::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeRayCastFunction::setInput(path, inputData);
}

bool CVtkVolumeRayCastMIPFunction::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeRayCastFunction::removeInput(path, inputData);
}

bool CVtkVolumeRayCastMIPFunction::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeRayCastFunction::fetchOutput(path, outputData);
}

bool CVtkVolumeRayCastMIPFunction::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeRayCastFunction::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkVolumeRayCastMIPFunction::propertyCount()
{
    return 0;
}

QString CVtkVolumeRayCastMIPFunction::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVolumeRayCastMIPFunction::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastMIPFunction::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVolumeRayCastMIPFunction::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVolumeRayCastMIPFunction::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVolumeRayCastMIPFunction::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVolumeRayCastMIPFunction::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVolumeRayCastMIPFunction::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVolumeRayCastMIPFunction::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVolumeRayCastMIPFunction::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastMIPFunction::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVolumeRayCastMIPFunction::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVolumeRayCastMIPFunction::setVtkObject(vtkObject* ptr)
{
    d->volumeRayCastMIPFunction = dynamic_cast<vtkVolumeRayCastMIPFunction*>(ptr);
    CVtkVolumeRayCastFunction::setVtkObject(ptr);
}





