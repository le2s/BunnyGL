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

#include "CVtkImageExtractComponents.h"
#include "ImagingAlgorithmsComponent.h"
#include "vtkImageExtractComponents.h"

DEFINE_VIS_NODE(CVtkImageExtractComponents, CVtkThreadedImageAlgorithm)
{
    pDesc->setNodeClassCategory("ImagingAlgorithms");
    pDesc->setNodeClassName("vtkImageExtractComponents");
    pDesc->setNodeClassDescription("vtkImageExtractComponents");
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

struct CVtkImageExtractComponentsData
{
    vtkImageExtractComponents *imageExtractComponents;
};

CVtkImageExtractComponents::CVtkImageExtractComponents()
{
    CVtkImageExtractComponents::InitializeNodeDesc();
    d = new CVtkImageExtractComponentsData;

    d->imageExtractComponents = 0;
    setVtkObject( vtkImageExtractComponents::New() );
}

CVtkImageExtractComponents::~CVtkImageExtractComponents()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->imageExtractComponents->Delete();
    delete d;
}


void CVtkImageExtractComponents::setComponents(const QList<QVariant> & com)
{
    if(com.count() == 1)
        d->imageExtractComponents->SetComponents(com[0].toInt());
    else if(com.count() == 2)
        d->imageExtractComponents->SetComponents(com[0].toInt(), com[1].toInt());
    else if(com.count() == 3)
        d->imageExtractComponents->SetComponents(com[0].toInt(), com[1].toInt(), com[2].toInt());
}

QList<QVariant> CVtkImageExtractComponents::components() const
{
    int components[3];
    d->imageExtractComponents->GetComponents(components);
    return QList<QVariant>() << components[0] << components[1] << components[2];
}

int CVtkImageExtractComponents::numberOfComponents()
{
    return d->imageExtractComponents->GetNumberOfComponents();
}


bool CVtkImageExtractComponents::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::hasInput(path);
}

bool CVtkImageExtractComponents::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::setInput(path, inputData);
}

bool CVtkImageExtractComponents::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::removeInput(path, inputData);
}

bool CVtkImageExtractComponents::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkThreadedImageAlgorithm::fetchOutput(path, outputData);
}

bool CVtkImageExtractComponents::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkImageExtractComponents::propertyCount()
{
    return 0;
}

QString CVtkImageExtractComponents::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkImageExtractComponents::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageExtractComponents::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkImageExtractComponents::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkImageExtractComponents::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkImageExtractComponents::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkImageExtractComponents::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkImageExtractComponents::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkImageExtractComponents::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkImageExtractComponents::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageExtractComponents::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkImageExtractComponents::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkImageExtractComponents::setVtkObject(vtkObject* ptr)
{
    d->imageExtractComponents = dynamic_cast<vtkImageExtractComponents*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->imageExtractComponents);
    CVtkThreadedImageAlgorithm::setVtkObject(ptr);
}





