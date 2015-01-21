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

#include "CVtkUnstructuredGridBunykRayCastFunction.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkUnstructuredGridBunykRayCastFunction.h"

DEFINE_VIS_NODE(CVtkUnstructuredGridBunykRayCastFunction, CVtkUnstructuredGridVolumeRayCastFunction)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkUnstructuredGridBunykRayCastFunction");
    pDesc->setNodeClassDescription("vtkUnstructuredGridBunykRayCastFunction");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

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

struct CVtkUnstructuredGridBunykRayCastFunctionData
{
    vtkUnstructuredGridBunykRayCastFunction *unstructuredGridBunykRayCastFunction;
};

CVtkUnstructuredGridBunykRayCastFunction::CVtkUnstructuredGridBunykRayCastFunction()
{
    CVtkUnstructuredGridBunykRayCastFunction::InitializeNodeDesc();
    d = new CVtkUnstructuredGridBunykRayCastFunctionData;

    d->unstructuredGridBunykRayCastFunction = 0;
    setVtkObject( vtkUnstructuredGridBunykRayCastFunction::New() );
}

CVtkUnstructuredGridBunykRayCastFunction::~CVtkUnstructuredGridBunykRayCastFunction()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->unstructuredGridBunykRayCastFunction->Delete();
    delete d;
}

bool CVtkUnstructuredGridBunykRayCastFunction::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayCastFunction::hasInput(path);
}

bool CVtkUnstructuredGridBunykRayCastFunction::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayCastFunction::setInput(path, inputData);
}

bool CVtkUnstructuredGridBunykRayCastFunction::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayCastFunction::removeInput(path, inputData);
}

bool CVtkUnstructuredGridBunykRayCastFunction::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeRayCastFunction::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridBunykRayCastFunction::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeRayCastFunction::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkUnstructuredGridBunykRayCastFunction::propertyCount()
{
    return 0;
}

QString CVtkUnstructuredGridBunykRayCastFunction::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkUnstructuredGridBunykRayCastFunction::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridBunykRayCastFunction::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkUnstructuredGridBunykRayCastFunction::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkUnstructuredGridBunykRayCastFunction::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkUnstructuredGridBunykRayCastFunction::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkUnstructuredGridBunykRayCastFunction::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkUnstructuredGridBunykRayCastFunction::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkUnstructuredGridBunykRayCastFunction::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkUnstructuredGridBunykRayCastFunction::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridBunykRayCastFunction::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkUnstructuredGridBunykRayCastFunction::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkUnstructuredGridBunykRayCastFunction::setVtkObject(vtkObject* ptr)
{
    d->unstructuredGridBunykRayCastFunction = dynamic_cast<vtkUnstructuredGridBunykRayCastFunction*>(ptr);
    CVtkUnstructuredGridVolumeRayCastFunction::setVtkObject(ptr);
}





