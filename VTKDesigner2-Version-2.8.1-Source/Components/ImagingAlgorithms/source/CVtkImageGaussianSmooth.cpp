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

#include "CVtkImageGaussianSmooth.h"
#include "ImagingAlgorithmsComponent.h"
#include "vtkImageGaussianSmooth.h"

DEFINE_VIS_NODE(CVtkImageGaussianSmooth, CVtkThreadedImageAlgorithm)
{
    pDesc->setNodeClassCategory("ImagingAlgorithms");
    pDesc->setNodeClassName("vtkImageGaussianSmooth");
    pDesc->setNodeClassDescription("vtkImageGaussianSmooth");
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

struct CVtkImageGaussianSmoothData
{
    vtkImageGaussianSmooth *imageGaussianSmooth;
};

CVtkImageGaussianSmooth::CVtkImageGaussianSmooth()
{
    CVtkImageGaussianSmooth::InitializeNodeDesc();
    d = new CVtkImageGaussianSmoothData;

    d->imageGaussianSmooth = 0;
    setVtkObject( vtkImageGaussianSmooth::New() );
}

CVtkImageGaussianSmooth::~CVtkImageGaussianSmooth()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->imageGaussianSmooth->Delete();
    delete d;
}

void CVtkImageGaussianSmooth::setStandardDeviations(const QList<QVariant>& val)
{
    if(val.count() == 1)
        d->imageGaussianSmooth->SetStandardDeviation(val[0].toDouble());
    else if(val.count() == 2)
        d->imageGaussianSmooth->SetStandardDeviation(val[0].toDouble(), val[1].toDouble());
    else if(val.count() == 3)
        d->imageGaussianSmooth->SetStandardDeviation(val[0].toDouble(), val[1].toDouble(), val[2].toDouble());
}

QList<QVariant> CVtkImageGaussianSmooth::standardDeviations() const
{
    double dev[3];
    d->imageGaussianSmooth->GetStandardDeviations(dev);
    return QList<QVariant>() << dev[0] << dev[1] << dev[2];
}


void CVtkImageGaussianSmooth::setRadiusFactors(const QList<QVariant>& val)
{
    if(val.count() == 1)
        d->imageGaussianSmooth->SetRadiusFactor(val[0].toDouble());
    else if(val.count() == 2)
        d->imageGaussianSmooth->SetRadiusFactors(val[0].toDouble(), val[1].toDouble());
    else if(val.count() == 3)
        d->imageGaussianSmooth->SetRadiusFactors(val[0].toDouble(), val[1].toDouble(), val[2].toDouble());
}

QList<QVariant> CVtkImageGaussianSmooth::radiusFactors() const
{
    double redius[3];
    d->imageGaussianSmooth->GetRadiusFactors(redius);
    return QList<QVariant>() << redius[0] << redius[1] << redius[2];
}

void CVtkImageGaussianSmooth::setDimensionality(int val)
{
    d->imageGaussianSmooth->SetDimensionality(val);
}

int CVtkImageGaussianSmooth::dimensionality() const
{
    return d->imageGaussianSmooth->GetDimensionality();
}

bool CVtkImageGaussianSmooth::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::hasInput(path);
}

bool CVtkImageGaussianSmooth::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::setInput(path, inputData);
}

bool CVtkImageGaussianSmooth::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::removeInput(path, inputData);
}

bool CVtkImageGaussianSmooth::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkThreadedImageAlgorithm::fetchOutput(path, outputData);
}

bool CVtkImageGaussianSmooth::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkImageGaussianSmooth::propertyCount()
{
    return 0;
}

QString CVtkImageGaussianSmooth::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkImageGaussianSmooth::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageGaussianSmooth::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkImageGaussianSmooth::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkImageGaussianSmooth::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkImageGaussianSmooth::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkImageGaussianSmooth::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkImageGaussianSmooth::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkImageGaussianSmooth::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkImageGaussianSmooth::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageGaussianSmooth::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkImageGaussianSmooth::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkImageGaussianSmooth::setVtkObject(vtkObject* ptr)
{
    d->imageGaussianSmooth = dynamic_cast<vtkImageGaussianSmooth*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->imageGaussianSmooth);
    CVtkThreadedImageAlgorithm::setVtkObject(ptr);
}





