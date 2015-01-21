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

#include "CVtkUnstructuredGridPartialPreIntegration.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkUnstructuredGridPartialPreIntegration.h"

DEFINE_VIS_NODE(CVtkUnstructuredGridPartialPreIntegration, CVtkUnstructuredGridVolumeRayIntegrator)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkUnstructuredGridPartialPreIntegration");
    pDesc->setNodeClassDescription("vtkUnstructuredGridPartialPreIntegration");
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

struct CVtkUnstructuredGridPartialPreIntegrationData
{
    vtkUnstructuredGridPartialPreIntegration *unstructuredGridPartialPreIntegration;
};

CVtkUnstructuredGridPartialPreIntegration::CVtkUnstructuredGridPartialPreIntegration()
{
    CVtkUnstructuredGridPartialPreIntegration::InitializeNodeDesc();
    d = new CVtkUnstructuredGridPartialPreIntegrationData;

    d->unstructuredGridPartialPreIntegration = 0;
    setVtkObject( vtkUnstructuredGridPartialPreIntegration::New() );
}

CVtkUnstructuredGridPartialPreIntegration::~CVtkUnstructuredGridPartialPreIntegration()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->unstructuredGridPartialPreIntegration->Delete();
    delete d;
}

bool CVtkUnstructuredGridPartialPreIntegration::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::hasInput(path);
}

bool CVtkUnstructuredGridPartialPreIntegration::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::setInput(path, inputData);
}

bool CVtkUnstructuredGridPartialPreIntegration::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::removeInput(path, inputData);
}

bool CVtkUnstructuredGridPartialPreIntegration::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridPartialPreIntegration::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkUnstructuredGridPartialPreIntegration::propertyCount()
{
    return 0;
}

QString CVtkUnstructuredGridPartialPreIntegration::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkUnstructuredGridPartialPreIntegration::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridPartialPreIntegration::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkUnstructuredGridPartialPreIntegration::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkUnstructuredGridPartialPreIntegration::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkUnstructuredGridPartialPreIntegration::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkUnstructuredGridPartialPreIntegration::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkUnstructuredGridPartialPreIntegration::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkUnstructuredGridPartialPreIntegration::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkUnstructuredGridPartialPreIntegration::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridPartialPreIntegration::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkUnstructuredGridPartialPreIntegration::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkUnstructuredGridPartialPreIntegration::setVtkObject(vtkObject* ptr)
{
    d->unstructuredGridPartialPreIntegration = dynamic_cast<vtkUnstructuredGridPartialPreIntegration*>(ptr);
    CVtkUnstructuredGridVolumeRayIntegrator::setVtkObject(ptr);
}





