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

#include "CVtkVolumeRayCastCompositeFunction.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkVolumeRayCastCompositeFunction.h"

DEFINE_VIS_NODE(CVtkVolumeRayCastCompositeFunction, CVtkVolumeRayCastFunction)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVolumeRayCastCompositeFunction");
    pDesc->setNodeClassDescription("vtkVolumeRayCastCompositeFunction");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("CompositeMethod", "SetCompositeMethod", "GetCompositeMethod", QStringList() << "int", QStringList(), "void", "int");
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

struct CVtkVolumeRayCastCompositeFunctionData
{
    vtkVolumeRayCastCompositeFunction *volumeRayCastCompositeFunction;
};

CVtkVolumeRayCastCompositeFunction::CVtkVolumeRayCastCompositeFunction()
{
    CVtkVolumeRayCastCompositeFunction::InitializeNodeDesc();
    d = new CVtkVolumeRayCastCompositeFunctionData;

    d->volumeRayCastCompositeFunction = 0;
    setVtkObject( vtkVolumeRayCastCompositeFunction::New() );
}

CVtkVolumeRayCastCompositeFunction::~CVtkVolumeRayCastCompositeFunction()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->volumeRayCastCompositeFunction->Delete();
    delete d;
}

void CVtkVolumeRayCastCompositeFunction::setCompositeMethod(CVtkVolumeRayCastCompositeFunction::CompositeMethod m)
{
    d->volumeRayCastCompositeFunction->SetCompositeMethod( int(m) );
}

CVtkVolumeRayCastCompositeFunction::CompositeMethod CVtkVolumeRayCastCompositeFunction::compositeMethod() const
{
    return CVtkVolumeRayCastCompositeFunction::CompositeMethod(
                d->volumeRayCastCompositeFunction->GetCompositeMethod()
            );
}

bool CVtkVolumeRayCastCompositeFunction::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeRayCastFunction::hasInput(path);
}

bool CVtkVolumeRayCastCompositeFunction::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeRayCastFunction::setInput(path, inputData);
}

bool CVtkVolumeRayCastCompositeFunction::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeRayCastFunction::removeInput(path, inputData);
}

bool CVtkVolumeRayCastCompositeFunction::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeRayCastFunction::fetchOutput(path, outputData);
}

bool CVtkVolumeRayCastCompositeFunction::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkVolumeRayCastCompositeFunction::propertyCount()
{
    return 0;
}

QString CVtkVolumeRayCastCompositeFunction::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVolumeRayCastCompositeFunction::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastCompositeFunction::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVolumeRayCastCompositeFunction::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVolumeRayCastCompositeFunction::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVolumeRayCastCompositeFunction::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVolumeRayCastCompositeFunction::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVolumeRayCastCompositeFunction::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVolumeRayCastCompositeFunction::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVolumeRayCastCompositeFunction::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastCompositeFunction::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVolumeRayCastCompositeFunction::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVolumeRayCastCompositeFunction::setVtkObject(vtkObject* ptr)
{
    d->volumeRayCastCompositeFunction = dynamic_cast<vtkVolumeRayCastCompositeFunction*>(ptr);
    CVtkVolumeRayCastFunction::setVtkObject(ptr);
}





