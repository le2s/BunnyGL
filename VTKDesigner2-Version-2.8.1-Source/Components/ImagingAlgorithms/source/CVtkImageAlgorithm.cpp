/****************************************************************************
**
** Copyright (C) VCreate Logic Pvt Ltd
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Pvt Ltd.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkImageAlgorithm.h"
#include "ImagingAlgorithmsComponent.h"
#include "vtkImageAlgorithm.h"

#include "vtkDataObject.h"
#include "CVtkImageData.h"

DEFINE_VIS_NODE(CVtkImageAlgorithm, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("ImagingAlgorithms");
    pDesc->setNodeClassName("vtkImageAlgorithm");
    pDesc->setNodeClassDescription("vtkImageAlgorithm");
    pDesc->setNodeIcon( ImagingAlgorithmsComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "DataObjectInput",                          // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkDataObject",                            // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "ImageDataOutput",                          // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkImageData",                             // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkImageAlgorithmData
{
    vtkImageAlgorithm *imageAlgorithm;
    CVtkImageData outputData;
};

CVtkImageAlgorithm::CVtkImageAlgorithm()
{
    CVtkImageAlgorithm::InitializeNodeDesc();
    d = new CVtkImageAlgorithmData;

    d->imageAlgorithm = 0;
    // setVtkObject( vtkImageAlgorithm::New() );
}

CVtkImageAlgorithm::~CVtkImageAlgorithm()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    // d->imageAlgorithm->Delete();
    delete d;
}

bool CVtkImageAlgorithm::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "DataObjectInput")
    {
        if(!d->imageAlgorithm)
            return false;

        if(d->imageAlgorithm->GetInput())
            return true;

        return false;
    }

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkImageAlgorithm::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DataObjectInput")
    {
        if(!d->imageAlgorithm)
            return false;

        IVtkDataObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkDataObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkDataObject* object = data->getVtkDataObject();
            d->imageAlgorithm->SetInput(object);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkImageAlgorithm::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DataObjectInput")
    {
        if(!d->imageAlgorithm)
            return false;

        d->imageAlgorithm->SetInput(0);
        return true;
    }


    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkImageAlgorithm::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ImageDataOutput")
    {
        if(!d->imageAlgorithm)
            return false;

        d->outputData.setImageData(d->imageAlgorithm->GetOutput());
        *outputData = &(d->outputData);
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkImageAlgorithm::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ImageDataOutput")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkImageAlgorithm::propertyCount()
{
    return 0;
}

QString CVtkImageAlgorithm::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkImageAlgorithm::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageAlgorithm::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkImageAlgorithm::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkImageAlgorithm::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkImageAlgorithm::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkImageAlgorithm::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkImageAlgorithm::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkImageAlgorithm::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkImageAlgorithm::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageAlgorithm::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkImageAlgorithm::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkImageAlgorithm::setVtkObject(vtkObject* ptr)
{
    d->imageAlgorithm = dynamic_cast<vtkImageAlgorithm*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->imageAlgorithm);
}





