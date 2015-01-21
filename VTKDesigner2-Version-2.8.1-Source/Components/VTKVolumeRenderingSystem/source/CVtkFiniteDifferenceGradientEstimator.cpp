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

#include "CVtkFiniteDifferenceGradientEstimator.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkFiniteDifferenceGradientEstimator.h"

DEFINE_VIS_NODE(CVtkFiniteDifferenceGradientEstimator, CVtkEncodedGradientEstimator)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkFiniteDifferenceGradientEstimator");
    pDesc->setNodeClassDescription("vtkFiniteDifferenceGradientEstimator");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("SampleSpacingInVoxels", "SetSampleSpacingInVoxels", "GetSampleSpacingInVoxels", QStringList() << "int", QStringList(), "void", "int");
    
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

struct CVtkFiniteDifferenceGradientEstimatorData
{
    vtkFiniteDifferenceGradientEstimator *finiteDifferenceGradientEstimator;
};

CVtkFiniteDifferenceGradientEstimator::CVtkFiniteDifferenceGradientEstimator()
{
    CVtkFiniteDifferenceGradientEstimator::InitializeNodeDesc();
    d = new CVtkFiniteDifferenceGradientEstimatorData;

    d->finiteDifferenceGradientEstimator = 0;
    setVtkObject( vtkFiniteDifferenceGradientEstimator::New() );
}

CVtkFiniteDifferenceGradientEstimator::~CVtkFiniteDifferenceGradientEstimator()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->finiteDifferenceGradientEstimator->Delete();
    delete d;
}

void CVtkFiniteDifferenceGradientEstimator::setSampleSpacingInVoxels(int val)
{
    d->finiteDifferenceGradientEstimator->SetSampleSpacingInVoxels(val);
}

int CVtkFiniteDifferenceGradientEstimator::sampleSpacingInVoxels() const
{
    return d->finiteDifferenceGradientEstimator->GetSampleSpacingInVoxels();
}

bool CVtkFiniteDifferenceGradientEstimator::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkEncodedGradientEstimator::hasInput(path);
}

bool CVtkFiniteDifferenceGradientEstimator::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkEncodedGradientEstimator::setInput(path, inputData);
}

bool CVtkFiniteDifferenceGradientEstimator::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkEncodedGradientEstimator::removeInput(path, inputData);
}

bool CVtkFiniteDifferenceGradientEstimator::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkEncodedGradientEstimator::fetchOutput(path, outputData);
}

bool CVtkFiniteDifferenceGradientEstimator::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkEncodedGradientEstimator::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkFiniteDifferenceGradientEstimator::propertyCount()
{
    return 0;
}

QString CVtkFiniteDifferenceGradientEstimator::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkFiniteDifferenceGradientEstimator::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkFiniteDifferenceGradientEstimator::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkFiniteDifferenceGradientEstimator::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkFiniteDifferenceGradientEstimator::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkFiniteDifferenceGradientEstimator::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkFiniteDifferenceGradientEstimator::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkFiniteDifferenceGradientEstimator::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkFiniteDifferenceGradientEstimator::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkFiniteDifferenceGradientEstimator::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkFiniteDifferenceGradientEstimator::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkFiniteDifferenceGradientEstimator::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkFiniteDifferenceGradientEstimator::setVtkObject(vtkObject* ptr)
{
    d->finiteDifferenceGradientEstimator = dynamic_cast<vtkFiniteDifferenceGradientEstimator*>(ptr);
    CVtkEncodedGradientEstimator::setVtkObject(ptr);
}





