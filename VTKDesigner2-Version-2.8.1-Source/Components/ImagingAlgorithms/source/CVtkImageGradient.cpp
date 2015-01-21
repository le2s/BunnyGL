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

#include "CVtkImageGradient.h"
#include "ImagingAlgorithmsComponent.h"
#include "vtkImageGradient.h"

DEFINE_VIS_NODE(CVtkImageGradient, CVtkThreadedImageAlgorithm)
{
    pDesc->setNodeClassCategory("ImagingAlgorithms");
    pDesc->setNodeClassName("vtkImageGradient");
    pDesc->setNodeClassDescription("vtkImageGradient");
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

struct CVtkImageGradientData
{
    vtkImageGradient *imageGradient;
};

CVtkImageGradient::CVtkImageGradient()
{
    CVtkImageGradient::InitializeNodeDesc();
    d = new CVtkImageGradientData;

    d->imageGradient = 0;
    setVtkObject( vtkImageGradient::New() );
}

CVtkImageGradient::~CVtkImageGradient()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->imageGradient->Delete();
    delete d;
}

void CVtkImageGradient::setHandleBoundaries(bool val)
{
    d->imageGradient->SetHandleBoundaries(val);
}

bool CVtkImageGradient::isHandleBoundaries() const
{
    return d->imageGradient->GetHandleBoundaries();
}

void CVtkImageGradient::setDimensionality(int val)
{
    d->imageGradient->SetDimensionality(val);
}

int CVtkImageGradient::dimensionality() const
{
    return d->imageGradient->GetDimensionality();
}

bool CVtkImageGradient::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::hasInput(path);
}

bool CVtkImageGradient::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::setInput(path, inputData);
}

bool CVtkImageGradient::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::removeInput(path, inputData);
}

bool CVtkImageGradient::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkThreadedImageAlgorithm::fetchOutput(path, outputData);
}

bool CVtkImageGradient::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkImageGradient::propertyCount()
{
    return 0;
}

QString CVtkImageGradient::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkImageGradient::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageGradient::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkImageGradient::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkImageGradient::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkImageGradient::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkImageGradient::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkImageGradient::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkImageGradient::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkImageGradient::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageGradient::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkImageGradient::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkImageGradient::setVtkObject(vtkObject* ptr)
{
    d->imageGradient = dynamic_cast<vtkImageGradient*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->imageGradient);
    CVtkThreadedImageAlgorithm::setVtkObject(ptr);
}





