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

#include "CVtkImageGradientMagnitude.h"
#include "ImagingAlgorithmsComponent.h"
#include "vtkImageGradientMagnitude.h"

DEFINE_VIS_NODE(CVtkImageGradientMagnitude, CVtkThreadedImageAlgorithm)
{
    pDesc->setNodeClassCategory("ImagingAlgorithms");
    pDesc->setNodeClassName("vtkImageGradientMagnitude");
    pDesc->setNodeClassDescription("vtkImageGradientMagnitude");
    pDesc->setNodeIcon( ImagingAlgorithmsComponent::instance().nodeIcon() );

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

struct CVtkImageGradientMagnitudeData
{
    vtkImageGradientMagnitude *imageGradientMagnitude;
};

CVtkImageGradientMagnitude::CVtkImageGradientMagnitude()
{
    CVtkImageGradientMagnitude::InitializeNodeDesc();
    d = new CVtkImageGradientMagnitudeData;

    d->imageGradientMagnitude = 0;
    setVtkObject( vtkImageGradientMagnitude::New() );
}

CVtkImageGradientMagnitude::~CVtkImageGradientMagnitude()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->imageGradientMagnitude->Delete();
    delete d;
}

void CVtkImageGradientMagnitude::setHandleBoundaries(bool val)
{
    d->imageGradientMagnitude->SetHandleBoundaries(val);
}

bool CVtkImageGradientMagnitude::isHandleBoundaries() const
{
    return d->imageGradientMagnitude->GetHandleBoundaries();
}

void CVtkImageGradientMagnitude::setDimensionality(int val)
{
    d->imageGradientMagnitude->SetDimensionality(val);
}

int CVtkImageGradientMagnitude::dimensionality() const
{
    return d->imageGradientMagnitude->GetDimensionality();
}

bool CVtkImageGradientMagnitude::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::hasInput(path);
}

bool CVtkImageGradientMagnitude::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::setInput(path, inputData);
}

bool CVtkImageGradientMagnitude::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::removeInput(path, inputData);
}

bool CVtkImageGradientMagnitude::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkThreadedImageAlgorithm::fetchOutput(path, outputData);
}

bool CVtkImageGradientMagnitude::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkThreadedImageAlgorithm::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkImageGradientMagnitude::propertyCount()
{
    return 0;
}

QString CVtkImageGradientMagnitude::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkImageGradientMagnitude::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageGradientMagnitude::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkImageGradientMagnitude::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkImageGradientMagnitude::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkImageGradientMagnitude::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkImageGradientMagnitude::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkImageGradientMagnitude::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkImageGradientMagnitude::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkImageGradientMagnitude::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageGradientMagnitude::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkImageGradientMagnitude::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkImageGradientMagnitude::setVtkObject(vtkObject* ptr)
{
    d->imageGradientMagnitude = dynamic_cast<vtkImageGradientMagnitude*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->imageGradientMagnitude);
    CVtkThreadedImageAlgorithm::setVtkObject(ptr);
}





