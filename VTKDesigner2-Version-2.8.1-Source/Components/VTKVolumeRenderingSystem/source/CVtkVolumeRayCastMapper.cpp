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

#include "CVtkVolumeRayCastMapper.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkVolumeRayCastMapper.h"

#include "IVtkObjectIOData.h"
#include "vtkVolumeRayCastFunction.h"
#include "vtkEncodedGradientEstimator.h"

DEFINE_VIS_NODE(CVtkVolumeRayCastMapper, CVtkVolumeMapper)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVolumeRayCastMapper");
    pDesc->setNodeClassDescription("vtkVolumeRayCastMapper");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("SampleDistance", "SetSampleDistance", "GetSampleDistance", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Ambient", "SetImageSampleDistance", "GetImageSampleDistance", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("MinimumImageSampleDistance", "SetMinimumImageSampleDistance", "GetMinimumImageSampleDistance", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("MaximumImageSampleDistance", "SetMaximumImageSampleDistance", "GetMaximumImageSampleDistance", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("AutoAdjustSampleDistances", "SetAutoAdjustSampleDistances", "GetAutoAdjustSampleDistances", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("NumberOfThreads", "setNumberOfThreads", "numberOfThreads", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("IntermixIntersectingGeometry", "SetIntermixIntersectingGeometry", "GetIntermixIntersectingGeometry", QStringList() << "bool", QStringList(), "void", "bool");

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Volume RayCast Function",                  // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkVolumeRayCastFunction",                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Gradient Estimator",                       // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkEncodedGradientEstimator",              // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkVolumeRayCastMapperData
{
    vtkVolumeRayCastMapper *volumeRayCastMapper;
    bool hasRayCastFunction;
    bool hasGradientEstimator;
};

CVtkVolumeRayCastMapper::CVtkVolumeRayCastMapper()
{
    CVtkVolumeRayCastMapper::InitializeNodeDesc();
    d = new CVtkVolumeRayCastMapperData;

    d->volumeRayCastMapper = 0;
    d->hasRayCastFunction = false;
    d->hasGradientEstimator = false;
    setVtkObject( vtkVolumeRayCastMapper::New() );
}

CVtkVolumeRayCastMapper::~CVtkVolumeRayCastMapper()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->volumeRayCastMapper->Delete();
    delete d;
}


double CVtkVolumeRayCastMapper::sampleDistance() const
{
	return d->volumeRayCastMapper->GetSampleDistance();
}

void CVtkVolumeRayCastMapper::setSampleDistance(double val)
{
	d->volumeRayCastMapper->SetSampleDistance(val);
}

double CVtkVolumeRayCastMapper::imageSampleDistance() const
{
	return d->volumeRayCastMapper->GetImageSampleDistance();
}

void CVtkVolumeRayCastMapper::setImageSampleDistance(double val)
{
	d->volumeRayCastMapper->SetImageSampleDistance(val);
}

double CVtkVolumeRayCastMapper::minimumImageSampleDistance() const
{
	return d->volumeRayCastMapper->GetMinimumImageSampleDistance();
}

void CVtkVolumeRayCastMapper::setMinimumImageSampleDistance(double val)
{
	d->volumeRayCastMapper->SetMinimumImageSampleDistance(val);
}

double CVtkVolumeRayCastMapper::maximumImageSampleDistance() const
{
	return d->volumeRayCastMapper->GetMaximumImageSampleDistance();
}

void CVtkVolumeRayCastMapper::setMaximumImageSampleDistance(double val)
{
	d->volumeRayCastMapper->SetMaximumImageSampleDistance(val);
}

bool CVtkVolumeRayCastMapper::isAutoAdjustSampleDistances() const
{
	return d->volumeRayCastMapper->GetAutoAdjustSampleDistances();
}

void CVtkVolumeRayCastMapper::setAutoAdjustSampleDistances(bool val)
{
	d->volumeRayCastMapper->SetAutoAdjustSampleDistances(val);
}

int CVtkVolumeRayCastMapper::numberOfThreads() const
{
	return d->volumeRayCastMapper->GetNumberOfThreads();
}

void CVtkVolumeRayCastMapper::setNumberOfThreads(int val)
{
	d->volumeRayCastMapper->SetNumberOfThreads(val);
}

bool CVtkVolumeRayCastMapper::isIntermixIntersectingGeometry() const
{
	return d->volumeRayCastMapper->GetIntermixIntersectingGeometry();
}
void CVtkVolumeRayCastMapper::setIntermixIntersectingGeometry(bool val)
{
	d->volumeRayCastMapper->SetIntermixIntersectingGeometry(val);
}


bool CVtkVolumeRayCastMapper::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Volume RayCast Function")
    {
        return d->hasRayCastFunction;
    }

    if(path->pathName() == "Gradient Estimator")
    {
        return d->hasGradientEstimator;
    }

    return CVtkVolumeMapper::hasInput(path);
}

bool CVtkVolumeRayCastMapper::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Volume RayCast Function")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* obj = data->getVtkObject();
            vtkVolumeRayCastFunction* fn = vtkVolumeRayCastFunction::SafeDownCast(obj);
            if(!fn)
                return false;

            d->volumeRayCastMapper->SetVolumeRayCastFunction(fn);
            d->hasRayCastFunction = true;
            return true;
        }

        return false;
    }

    if(path->pathName() == "Gradient Estimator")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* obj = data->getVtkObject();
            vtkEncodedGradientEstimator* fn = vtkEncodedGradientEstimator::SafeDownCast(obj);
            if(!fn)
                return false;

            d->volumeRayCastMapper->SetGradientEstimator(fn);
            d->hasGradientEstimator = true;
            return true;
        }

        return false;
    }

    return CVtkVolumeMapper::setInput(path, inputData);
}

bool CVtkVolumeRayCastMapper::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Volume RayCast Function")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* obj = data->getVtkObject();
            vtkVolumeRayCastFunction* fn = vtkVolumeRayCastFunction::SafeDownCast(obj);
            if(fn != d->volumeRayCastMapper->GetVolumeRayCastFunction())
                return false;

            d->volumeRayCastMapper->SetVolumeRayCastFunction(0);
            d->hasRayCastFunction = false;
            return true;
        }

        return false;
    }

    if(path->pathName() == "Gradient Estimator")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* obj = data->getVtkObject();
            vtkEncodedGradientEstimator* fn = vtkEncodedGradientEstimator::SafeDownCast(obj);
            if(fn != d->volumeRayCastMapper->GetGradientEstimator())
                return false;

            d->volumeRayCastMapper->SetGradientEstimator(0);
            d->hasGradientEstimator = false;
            return true;
        }

        return false;
    }

    return CVtkVolumeMapper::removeInput(path, inputData);
}

bool CVtkVolumeRayCastMapper::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeMapper::fetchOutput(path, outputData);
}

bool CVtkVolumeRayCastMapper::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeMapper::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkVolumeRayCastMapper::propertyCount()
{
    return 0;
}

QString CVtkVolumeRayCastMapper::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVolumeRayCastMapper::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastMapper::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVolumeRayCastMapper::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVolumeRayCastMapper::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVolumeRayCastMapper::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVolumeRayCastMapper::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVolumeRayCastMapper::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVolumeRayCastMapper::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVolumeRayCastMapper::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastMapper::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVolumeRayCastMapper::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVolumeRayCastMapper::setVtkObject(vtkObject* ptr)
{
    d->volumeRayCastMapper = dynamic_cast<vtkVolumeRayCastMapper*>(ptr);
    CVtkVolumeMapper::setVtkObject(ptr);
}





