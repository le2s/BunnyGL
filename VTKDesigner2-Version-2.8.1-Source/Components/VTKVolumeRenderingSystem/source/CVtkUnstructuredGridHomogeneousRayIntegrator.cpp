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

#include "CVtkUnstructuredGridHomogeneousRayIntegrator.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkUnstructuredGridHomogeneousRayIntegrator.h"

DEFINE_VIS_NODE(CVtkUnstructuredGridHomogeneousRayIntegrator, CVtkUnstructuredGridVolumeRayIntegrator)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkUnstructuredGridHomogeneousRayIntegrator");
    pDesc->setNodeClassDescription("vtkUnstructuredGridHomogeneousRayIntegrator");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("TransferFunctionTableSize", "SetTransferFunctionTableSize", "GetTransferFunctionTableSize", QStringList() << "int", QStringList(), "void", "int");
    
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

struct CVtkUnstructuredGridHomogeneousRayIntegratorData
{
    vtkUnstructuredGridHomogeneousRayIntegrator *unstructuredGridHomogeneousRayIntegrator;
};

CVtkUnstructuredGridHomogeneousRayIntegrator::CVtkUnstructuredGridHomogeneousRayIntegrator()
{
    CVtkUnstructuredGridHomogeneousRayIntegrator::InitializeNodeDesc();
    d = new CVtkUnstructuredGridHomogeneousRayIntegratorData;

    d->unstructuredGridHomogeneousRayIntegrator = 0;
    setVtkObject( vtkUnstructuredGridHomogeneousRayIntegrator::New() );
}

CVtkUnstructuredGridHomogeneousRayIntegrator::~CVtkUnstructuredGridHomogeneousRayIntegrator()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->unstructuredGridHomogeneousRayIntegrator->Delete();
    delete d;
}

void CVtkUnstructuredGridHomogeneousRayIntegrator::setTransferFunctionTableSize(int val)
{
    d->unstructuredGridHomogeneousRayIntegrator->SetTransferFunctionTableSize(val);
}

int CVtkUnstructuredGridHomogeneousRayIntegrator::transferFunctionTableSize() const
{
    return d->unstructuredGridHomogeneousRayIntegrator->GetTransferFunctionTableSize();
}

bool CVtkUnstructuredGridHomogeneousRayIntegrator::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::hasInput(path);
}

bool CVtkUnstructuredGridHomogeneousRayIntegrator::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::setInput(path, inputData);
}

bool CVtkUnstructuredGridHomogeneousRayIntegrator::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::removeInput(path, inputData);
}

bool CVtkUnstructuredGridHomogeneousRayIntegrator::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkUnstructuredGridVolumeRayIntegrator::fetchOutput(path, outputData);
}

bool CVtkUnstructuredGridHomogeneousRayIntegrator::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkUnstructuredGridHomogeneousRayIntegrator::propertyCount()
{
    return 0;
}

QString CVtkUnstructuredGridHomogeneousRayIntegrator::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkUnstructuredGridHomogeneousRayIntegrator::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridHomogeneousRayIntegrator::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkUnstructuredGridHomogeneousRayIntegrator::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkUnstructuredGridHomogeneousRayIntegrator::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkUnstructuredGridHomogeneousRayIntegrator::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkUnstructuredGridHomogeneousRayIntegrator::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkUnstructuredGridHomogeneousRayIntegrator::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkUnstructuredGridHomogeneousRayIntegrator::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkUnstructuredGridHomogeneousRayIntegrator::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkUnstructuredGridHomogeneousRayIntegrator::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkUnstructuredGridHomogeneousRayIntegrator::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkUnstructuredGridHomogeneousRayIntegrator::setVtkObject(vtkObject* ptr)
{
    d->unstructuredGridHomogeneousRayIntegrator = dynamic_cast<vtkUnstructuredGridHomogeneousRayIntegrator*>(ptr);
    CVtkUnstructuredGridVolumeRayIntegrator::setVtkObject(ptr);
}





