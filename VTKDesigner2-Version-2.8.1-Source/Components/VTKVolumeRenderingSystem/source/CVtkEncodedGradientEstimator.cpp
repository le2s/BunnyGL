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

#include "CVtkEncodedGradientEstimator.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkEncodedGradientEstimator.h"

#include "CVtkObjectIOData.h"

DEFINE_VIS_NODE(CVtkEncodedGradientEstimator, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkEncodedGradientEstimator");
    pDesc->setNodeClassDescription("vtkEncodedGradientEstimator");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("GradientMagnitudeScale", "SetGradientMagnitudeScale", "GetGradientMagnitudeScale", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("BoundsClip", "SetBoundsClip", "GetBoundsClip", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("NumberOfThreads", "SetNumberOfThreads", "GetNumberOfThreads", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ComputeGradientMagnitudes", "SetComputeGradientMagnitudes", "GetComputeGradientMagnitudes", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("CylinderClip", "SetCylinderClip", "GetCylinderClip", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("ZeroNormalThreshold", "SetZeroNormalThreshold", "GetZeroNormalThreshold", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("CylinderClip", "SetZeroPad", "GetZeroPad", QStringList() << "bool", QStringList(), "void", "bool");
    
    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Encoded Gradient Estimator",               // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkEncodedGradientEstimator",              // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->setCreateFunction(0);
}

struct CVtkEncodedGradientEstimatorData
{
    vtkEncodedGradientEstimator *encodedGradientEstimator;
    CVtkObjectIOData outputData;
};

CVtkEncodedGradientEstimator::CVtkEncodedGradientEstimator()
{
    CVtkEncodedGradientEstimator::InitializeNodeDesc();
    d = new CVtkEncodedGradientEstimatorData;

    d->encodedGradientEstimator = 0;
}

CVtkEncodedGradientEstimator::~CVtkEncodedGradientEstimator()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    delete d;
}

double CVtkEncodedGradientEstimator::gradientMagnitudeScale() const
{
	return d->encodedGradientEstimator->GetGradientMagnitudeScale();
}

void CVtkEncodedGradientEstimator::setGradientMagnitudeScale(double val)
{
	d->encodedGradientEstimator->SetGradientMagnitudeScale(val);
}

bool CVtkEncodedGradientEstimator::isBoundsClip() const
{
	return d->encodedGradientEstimator->GetBoundsClip();
}

void CVtkEncodedGradientEstimator::setBoundsClip(bool val)
{
	d->encodedGradientEstimator->SetBoundsClip(val);
}

void CVtkEncodedGradientEstimator::setBounds(QList<QVariant> val)
{
	d->encodedGradientEstimator->SetBounds(	val[0].toInt(),
											val[1].toInt(),
											val[2].toInt(),
											val[3].toInt(),
											val[4].toInt(),
											val[5].toInt());
}

QList<QVariant> CVtkEncodedGradientEstimator::bounds() const
{
	int* r = d->encodedGradientEstimator->GetBounds();

	QList<QVariant> ret = QList<QVariant>() << r[0] << r[1] << r[2] << r[3] << r[4] << r[5];
    return ret;
}

int CVtkEncodedGradientEstimator::numberOfThreads() const
{
	return d->encodedGradientEstimator->GetNumberOfThreads();
}

void CVtkEncodedGradientEstimator::setNumberOfThreads(int val)
{
	d->encodedGradientEstimator->SetNumberOfThreads(val);
}

bool CVtkEncodedGradientEstimator::isComputeGradientMagnitudes() const
{
	return d->encodedGradientEstimator->GetComputeGradientMagnitudes();
}

void CVtkEncodedGradientEstimator::setComputeGradientMagnitudes(bool val)
{
	d->encodedGradientEstimator->SetComputeGradientMagnitudes(val);
}

bool CVtkEncodedGradientEstimator::isCylinderClip() const
{
	return d->encodedGradientEstimator->GetCylinderClip();
}

void CVtkEncodedGradientEstimator::setCylinderClip(bool val)
{
	d->encodedGradientEstimator->SetCylinderClip(val);
}

double CVtkEncodedGradientEstimator::zeroNormalThreshold() const
{
	return d->encodedGradientEstimator->GetZeroNormalThreshold();
}

void CVtkEncodedGradientEstimator::setZeroNormalThreshold(double val)
{
	d->encodedGradientEstimator->SetZeroNormalThreshold(val);
}

bool CVtkEncodedGradientEstimator::isZeroPad() const
{
	return d->encodedGradientEstimator->GetZeroPad();
}

void CVtkEncodedGradientEstimator::setZeroPad(bool val)
{
	d->encodedGradientEstimator->SetZeroPad(val);
}

int CVtkEncodedGradientEstimator::useCylinderClip() const
{
	return d->encodedGradientEstimator->GetUseCylinderClip();
}

QList<QVariant> CVtkEncodedGradientEstimator::inputSize() const
{
	int* r = d->encodedGradientEstimator->GetInputSize();

	QList<QVariant> ret = QList<QVariant>() << r[0] << r[1] << r[2];
    return ret;
}

QList<QVariant> CVtkEncodedGradientEstimator::inputAspect() const
{
	float* r = d->encodedGradientEstimator->GetInputAspect();

	QList<QVariant> ret = QList<QVariant>() << double(r[0]) << double(r[1]) << double(r[2]);
    return ret;
}

double CVtkEncodedGradientEstimator::lastUpdateTimeInSeconds() const
{
	return d->encodedGradientEstimator->GetLastUpdateTimeInSeconds();
}

double CVtkEncodedGradientEstimator::lastUpdateTimeInCPUSeconds() const
{
	return d->encodedGradientEstimator->GetLastUpdateTimeInCPUSeconds();
}

bool CVtkEncodedGradientEstimator::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkEncodedGradientEstimator::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkEncodedGradientEstimator::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkEncodedGradientEstimator::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Encoded Gradient Estimator")
    {
        *outputData = &d->outputData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkEncodedGradientEstimator::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkEncodedGradientEstimator::propertyCount()
{
    return 0;
}

QString CVtkEncodedGradientEstimator::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkEncodedGradientEstimator::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkEncodedGradientEstimator::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkEncodedGradientEstimator::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkEncodedGradientEstimator::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkEncodedGradientEstimator::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkEncodedGradientEstimator::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkEncodedGradientEstimator::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkEncodedGradientEstimator::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkEncodedGradientEstimator::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkEncodedGradientEstimator::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkEncodedGradientEstimator::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkEncodedGradientEstimator::setVtkObject(vtkObject* ptr)
{
    d->encodedGradientEstimator = dynamic_cast<vtkEncodedGradientEstimator*>(ptr);
    d->outputData.setObject(ptr);
}





