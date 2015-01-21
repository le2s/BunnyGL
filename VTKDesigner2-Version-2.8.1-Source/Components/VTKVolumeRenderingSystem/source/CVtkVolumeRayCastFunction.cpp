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

#include "CVtkVolumeRayCastFunction.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkVolumeRayCastFunction.h"

#include "CVtkObjectIOData.h"

DEFINE_VIS_NODE(CVtkVolumeRayCastFunction, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVolumeRayCastFunction");
    pDesc->setNodeClassDescription("vtkVolumeRayCastFunction");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Volume RayCast Function",                  // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkVolumeRayCastFunction",                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->setCreateFunction(0);
}

struct CVtkVolumeRayCastFunctionData
{
    vtkVolumeRayCastFunction *volumeRayCastFunction;
    CVtkObjectIOData outputData;
};

CVtkVolumeRayCastFunction::CVtkVolumeRayCastFunction()
{
    CVtkVolumeRayCastFunction::InitializeNodeDesc();
    d = new CVtkVolumeRayCastFunctionData;

    d->volumeRayCastFunction = 0;
}

CVtkVolumeRayCastFunction::~CVtkVolumeRayCastFunction()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
}

bool CVtkVolumeRayCastFunction::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkVolumeRayCastFunction::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkVolumeRayCastFunction::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkVolumeRayCastFunction::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Volume RayCast Function")
    {
        *outputData = &d->outputData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkVolumeRayCastFunction::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkVolumeRayCastFunction::propertyCount()
{
    return 0;
}

QString CVtkVolumeRayCastFunction::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVolumeRayCastFunction::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastFunction::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVolumeRayCastFunction::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVolumeRayCastFunction::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVolumeRayCastFunction::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVolumeRayCastFunction::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVolumeRayCastFunction::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVolumeRayCastFunction::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVolumeRayCastFunction::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastFunction::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVolumeRayCastFunction::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVolumeRayCastFunction::setVtkObject(vtkObject* ptr)
{
    d->volumeRayCastFunction = dynamic_cast<vtkVolumeRayCastFunction*>(ptr);
    d->outputData.setObject(d->volumeRayCastFunction);
}





