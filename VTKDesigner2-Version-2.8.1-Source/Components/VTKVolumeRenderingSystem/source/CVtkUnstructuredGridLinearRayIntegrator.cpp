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

#include "CVtkUnstructuredGridLinearRayIntegrator.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkUnstructuredGridLinearRayIntegrator.h"

DEFINE_VIS_NODE(CVtkUnstructuredGridLinearRayIntegrator, CVtkUnstructuredGridVolumeRayIntegrator)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkUnstructuredGridLinearRayIntegrator");
    pDesc->setNodeClassDescription("vtkUnstructuredGridLinearRayIntegrator");
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

struct CVtkUnstructuredGridLinearRayIntegratorData
{
    vtkUnstructuredGridLinearRayIntegrator *unstructuredGridLinearRayIntegrator;
};

CVtkUnstructuredGridLinearRayIntegrator::CVtkUnstructuredGridLinearRayIntegrator()
{
    CVtkUnstructuredGridLinearRayIntegrator::InitializeNodeDesc();
    d = new CVtkUnstructuredGridLinearRayIntegratorData;

    d->unstructuredGridLinearRayIntegrator = 0;
    setVtkObject( vtkUnstructuredGridLinearRayIntegrator::New() );
}

CVtkUnstructuredGridLinearRayIntegrator::~CVtkUnstructuredGridLinearRayIntegrator()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->unstructuredGridLinearRayIntegrator->Delete();
    delete d;
}

bool CVtkUnstructuredGridLinearRayIntegrator::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::hasInput(path);
}

bool CVtkUnstructuredGridLinearRayIntegrator::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::setInput(path, inputData);
}

bool CVtkUnstructuredGridLinearRayIntegrator::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::removeInput(path, inputData);
}

bool CVtkUnstructuredGridLinearRayIntegrator::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridLinearRayIntegrator::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkUnstructuredGridLinearRayIntegrator::propertyCount()
{
    return 0;
}

QString CVtkUnstructuredGridLinearRayIntegrator::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkUnstructuredGridLinearRayIntegrator::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridLinearRayIntegrator::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkUnstructuredGridLinearRayIntegrator::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkUnstructuredGridLinearRayIntegrator::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkUnstructuredGridLinearRayIntegrator::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkUnstructuredGridLinearRayIntegrator::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkUnstructuredGridLinearRayIntegrator::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkUnstructuredGridLinearRayIntegrator::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkUnstructuredGridLinearRayIntegrator::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridLinearRayIntegrator::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkUnstructuredGridLinearRayIntegrator::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkUnstructuredGridLinearRayIntegrator::setVtkObject(vtkObject* ptr)
{
    d->unstructuredGridLinearRayIntegrator = dynamic_cast<vtkUnstructuredGridLinearRayIntegrator*>(ptr);
    CVtkUnstructuredGridVolumeRayIntegrator::setVtkObject(ptr);
}





