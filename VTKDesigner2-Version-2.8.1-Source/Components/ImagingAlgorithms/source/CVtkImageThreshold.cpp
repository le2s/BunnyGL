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

#include "CVtkImageThreshold.h"
#include "ImagingAlgorithmsComponent.h"
#include "vtkImageThreshold.h"

DEFINE_VIS_NODE(CVtkImageThreshold, CVtkThreadedImageAlgorithm)
{
    pDesc->setNodeClassCategory("ImagingAlgorithms");
    pDesc->setNodeClassName("vtkImageThreshold");
    pDesc->setNodeClassDescription("vtkImageThreshold");
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

struct CVtkImageThresholdData
{
    vtkImageThreshold *imageThreshold;
};

CVtkImageThreshold::CVtkImageThreshold()
{
    CVtkImageThreshold::InitializeNodeDesc();
    d = new CVtkImageThresholdData;

    d->imageThreshold = 0;
    setVtkObject( vtkImageThreshold::New() );
}

CVtkImageThreshold::~CVtkImageThreshold()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->imageThreshold->Delete();
    delete d;
}

void CVtkImageThreshold::setReplaceIn(bool val)
{
    d->imageThreshold->SetReplaceIn(val);
}

bool CVtkImageThreshold::isReplaceIn() const
{
    return d->imageThreshold->GetReplaceIn();
}

void CVtkImageThreshold::setInValue(double val)
{
    d->imageThreshold->SetInValue(val);
}

double CVtkImageThreshold::inValue() const
{
    return d->imageThreshold->GetInValue();
}

void CVtkImageThreshold::thresholdByLower(double val)
{
    d->imageThreshold->ThresholdByLower(val);
}

void CVtkImageThreshold::thresholdByUpper(double val)
{
    d->imageThreshold->ThresholdByUpper(val);
}

void CVtkImageThreshold::thresholdBetween(double lower, double upper)
{
    d->imageThreshold->ThresholdBetween(lower, upper);
}

double CVtkImageThreshold::upperThreshold() const
{
    return d->imageThreshold->GetUpperThreshold();
}

double CVtkImageThreshold::lowerThreshold() const
{
    return d->imageThreshold->GetLowerThreshold();
}

void CVtkImageThreshold::setOutputScalarType(CVtkImageThreshold::OutputScalarType type)
{
    d->imageThreshold->SetOutputScalarType( int(type) );
}

CVtkImageThreshold::OutputScalarType CVtkImageThreshold::outputScalarType() const
{
    return CVtkImageThreshold::OutputScalarType( d->imageThreshold->GetOutputScalarType() );
}

bool CVtkImageThreshold::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::hasInput(path);
}

bool CVtkImageThreshold::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::setInput(path, inputData);
}

bool CVtkImageThreshold::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::removeInput(path, inputData);
}

bool CVtkImageThreshold::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkThreadedImageAlgorithm::fetchOutput(path, outputData);
}

bool CVtkImageThreshold::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkImageThreshold::propertyCount()
{
    return 0;
}

QString CVtkImageThreshold::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkImageThreshold::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageThreshold::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkImageThreshold::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkImageThreshold::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkImageThreshold::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkImageThreshold::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkImageThreshold::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkImageThreshold::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkImageThreshold::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageThreshold::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkImageThreshold::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkImageThreshold::setVtkObject(vtkObject* ptr)
{
    d->imageThreshold = dynamic_cast<vtkImageThreshold*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->imageThreshold);
    CVtkThreadedImageAlgorithm::setVtkObject(ptr);
}





