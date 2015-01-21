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

#include "CVtkUnstructuredGridPreIntegration.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkUnstructuredGridPreIntegration.h"

DEFINE_VIS_NODE(CVtkUnstructuredGridPreIntegration, CVtkUnstructuredGridVolumeRayIntegrator)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkUnstructuredGridPreIntegration");
    pDesc->setNodeClassDescription("vtkUnstructuredGridPreIntegration");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("IntegrationTableScalarResolution", "SetIntegrationTableScalarResolution", "GetIntegrationTableScalarResolution", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("IntegrationTableLengthResolution", "SetIntegrationTableLengthResolution", "GetIntegrationTableLengthResolution", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("IncrementalPreIntegration", "SetIncrementalPreIntegration", "GetIncrementalPreIntegration", QStringList() << "bool", QStringList(), "void", "bool");
    
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

struct CVtkUnstructuredGridPreIntegrationData
{
    vtkUnstructuredGridPreIntegration *unstructuredGridPreIntegration;
};

CVtkUnstructuredGridPreIntegration::CVtkUnstructuredGridPreIntegration()
{
    CVtkUnstructuredGridPreIntegration::InitializeNodeDesc();
    d = new CVtkUnstructuredGridPreIntegrationData;

    d->unstructuredGridPreIntegration = 0;
    setVtkObject( vtkUnstructuredGridPreIntegration::New() );
}

CVtkUnstructuredGridPreIntegration::~CVtkUnstructuredGridPreIntegration()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->unstructuredGridPreIntegration->Delete();
    delete d;
}

void CVtkUnstructuredGridPreIntegration::setIntegrationTableScalarResolution(int val)
{
    d->unstructuredGridPreIntegration->SetIntegrationTableScalarResolution(val);
}

int CVtkUnstructuredGridPreIntegration::integrationTableScalarResolution() const
{
    return d->unstructuredGridPreIntegration->GetIntegrationTableScalarResolution();
}

void CVtkUnstructuredGridPreIntegration::setIntegrationTableLengthResolution(int val)
{
    d->unstructuredGridPreIntegration->SetIntegrationTableLengthResolution(val);
}

int CVtkUnstructuredGridPreIntegration::integrationTableLengthResolution() const
{
    return d->unstructuredGridPreIntegration->GetIntegrationTableLengthResolution();
}

void CVtkUnstructuredGridPreIntegration::setIncrementalPreIntegration(bool val)
{
    d->unstructuredGridPreIntegration->SetIncrementalPreIntegration(val);
}

bool CVtkUnstructuredGridPreIntegration::isIncrementalPreIntegration() const
{
    return d->unstructuredGridPreIntegration->GetIncrementalPreIntegration();
}

bool CVtkUnstructuredGridPreIntegration::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::hasInput(path);
}

bool CVtkUnstructuredGridPreIntegration::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::setInput(path, inputData);
}

bool CVtkUnstructuredGridPreIntegration::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::removeInput(path, inputData);
}

bool CVtkUnstructuredGridPreIntegration::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridPreIntegration::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkUnstructuredGridPreIntegration::propertyCount()
{
    return 0;
}

QString CVtkUnstructuredGridPreIntegration::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkUnstructuredGridPreIntegration::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridPreIntegration::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkUnstructuredGridPreIntegration::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkUnstructuredGridPreIntegration::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkUnstructuredGridPreIntegration::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkUnstructuredGridPreIntegration::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkUnstructuredGridPreIntegration::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkUnstructuredGridPreIntegration::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkUnstructuredGridPreIntegration::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridPreIntegration::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkUnstructuredGridPreIntegration::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkUnstructuredGridPreIntegration::setVtkObject(vtkObject* ptr)
{
    d->unstructuredGridPreIntegration = dynamic_cast<vtkUnstructuredGridPreIntegration*>(ptr);
    CVtkUnstructuredGridVolumeRayIntegrator::setVtkObject(ptr);
}





