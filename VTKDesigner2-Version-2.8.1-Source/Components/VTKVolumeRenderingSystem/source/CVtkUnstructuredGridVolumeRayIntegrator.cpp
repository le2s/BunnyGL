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

#include "CVtkUnstructuredGridVolumeRayIntegrator.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkUnstructuredGridVolumeRayIntegrator.h"

#include "CVtkObjectIOData.h"

DEFINE_VIS_NODE(CVtkUnstructuredGridVolumeRayIntegrator, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkUnstructuredGridVolumeRayIntegrator");
    pDesc->setNodeClassDescription("vtkUnstructuredGridVolumeRayIntegrator");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "UGrid Volume Ray Integrator",              // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkUnstructuredGridVolumeRayIntegrator",   // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->setCreateFunction(0);
}

struct CVtkUnstructuredGridVolumeRayIntegratorData
{
    vtkUnstructuredGridVolumeRayIntegrator *unstructuredGridVolumeRayIntegrator;
    CVtkObjectIOData outputData;
};

CVtkUnstructuredGridVolumeRayIntegrator::CVtkUnstructuredGridVolumeRayIntegrator()
{
    CVtkUnstructuredGridVolumeRayIntegrator::InitializeNodeDesc();
    d = new CVtkUnstructuredGridVolumeRayIntegratorData;

    d->unstructuredGridVolumeRayIntegrator = 0;
}

CVtkUnstructuredGridVolumeRayIntegrator::~CVtkUnstructuredGridVolumeRayIntegrator()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    delete d;
}

bool CVtkUnstructuredGridVolumeRayIntegrator::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkUnstructuredGridVolumeRayIntegrator::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkUnstructuredGridVolumeRayIntegrator::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkUnstructuredGridVolumeRayIntegrator::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "UGrid Volume Ray Integrator")
    {
        *outputData = &d->outputData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridVolumeRayIntegrator::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkUnstructuredGridVolumeRayIntegrator::propertyCount()
{
    return 0;
}

QString CVtkUnstructuredGridVolumeRayIntegrator::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkUnstructuredGridVolumeRayIntegrator::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridVolumeRayIntegrator::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkUnstructuredGridVolumeRayIntegrator::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkUnstructuredGridVolumeRayIntegrator::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkUnstructuredGridVolumeRayIntegrator::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkUnstructuredGridVolumeRayIntegrator::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkUnstructuredGridVolumeRayIntegrator::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkUnstructuredGridVolumeRayIntegrator::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkUnstructuredGridVolumeRayIntegrator::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridVolumeRayIntegrator::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkUnstructuredGridVolumeRayIntegrator::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkUnstructuredGridVolumeRayIntegrator::setVtkObject(vtkObject* ptr)
{
    d->unstructuredGridVolumeRayIntegrator = dynamic_cast<vtkUnstructuredGridVolumeRayIntegrator*>(ptr);
    d->outputData.setObject(d->unstructuredGridVolumeRayIntegrator);
}





