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

#include "CVtkThreadedImageAlgorithm.h"
#include "ImagingAlgorithmsComponent.h"
#include "vtkThreadedImageAlgorithm.h"

DEFINE_VIS_NODE(CVtkThreadedImageAlgorithm, CVtkImageAlgorithm)
{
    pDesc->setNodeClassCategory("ImagingAlgorithms");
    pDesc->setNodeClassName("vtkThreadedImageAlgorithm");
    pDesc->setNodeClassDescription("vtkThreadedImageAlgorithm");
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

struct CVtkThreadedImageAlgorithmData
{
    vtkThreadedImageAlgorithm *threadedImageAlgorithm;
};

CVtkThreadedImageAlgorithm::CVtkThreadedImageAlgorithm()
{
    CVtkThreadedImageAlgorithm::InitializeNodeDesc();
    d = new CVtkThreadedImageAlgorithmData;

    d->threadedImageAlgorithm = 0;
    // setVtkObject( vtkThreadedImageAlgorithm::New() );
}

CVtkThreadedImageAlgorithm::~CVtkThreadedImageAlgorithm()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    // d->threadedImageAlgorithm->Delete();
    delete d;
}

void CVtkThreadedImageAlgorithm::setNumberOfThreads(int count)
{
    if(d->threadedImageAlgorithm)
        d->threadedImageAlgorithm->SetNumberOfThreads(count);
}

int CVtkThreadedImageAlgorithm::numberOfThreads() const
{
    if(d->threadedImageAlgorithm)
        return d->threadedImageAlgorithm->GetNumberOfThreads();

    return 0;
}

bool CVtkThreadedImageAlgorithm::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkImageAlgorithm::hasInput(path);
}

bool CVtkThreadedImageAlgorithm::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkImageAlgorithm::setInput(path, inputData);
}

bool CVtkThreadedImageAlgorithm::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkImageAlgorithm::removeInput(path, inputData);
}

bool CVtkThreadedImageAlgorithm::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkImageAlgorithm::fetchOutput(path, outputData);
}

bool CVtkThreadedImageAlgorithm::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkImageAlgorithm::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkThreadedImageAlgorithm::propertyCount()
{
    return 0;
}

QString CVtkThreadedImageAlgorithm::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkThreadedImageAlgorithm::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkThreadedImageAlgorithm::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkThreadedImageAlgorithm::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkThreadedImageAlgorithm::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkThreadedImageAlgorithm::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkThreadedImageAlgorithm::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkThreadedImageAlgorithm::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkThreadedImageAlgorithm::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkThreadedImageAlgorithm::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkThreadedImageAlgorithm::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkThreadedImageAlgorithm::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkThreadedImageAlgorithm::setVtkObject(vtkObject* ptr)
{
    d->threadedImageAlgorithm = dynamic_cast<vtkThreadedImageAlgorithm*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->threadedImageAlgorithm);
    CVtkImageAlgorithm::setVtkObject(ptr);
}





