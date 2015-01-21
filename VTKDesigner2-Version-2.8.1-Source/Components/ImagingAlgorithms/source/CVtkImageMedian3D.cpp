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

#include "CVtkImageMedian3D.h"
#include "ImagingAlgorithmsComponent.h"
#include "vtkImageMedian3D.h"

DEFINE_VIS_NODE(CVtkImageMedian3D, CVtkThreadedImageAlgorithm)
{
    pDesc->setNodeClassCategory("ImagingAlgorithms");
    pDesc->setNodeClassName("vtkImageMedian3D");
    pDesc->setNodeClassDescription("vtkImageMedian3D");
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

struct CVtkImageMedian3DData
{
    vtkImageMedian3D *imageMedian3D;
};

CVtkImageMedian3D::CVtkImageMedian3D()
{
    CVtkImageMedian3D::InitializeNodeDesc();
    d = new CVtkImageMedian3DData;

    d->imageMedian3D = 0;
    setVtkObject( vtkImageMedian3D::New() );
}

CVtkImageMedian3D::~CVtkImageMedian3D()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->imageMedian3D->Delete();
    delete d;
}


/**
 \todo
*/
void CVtkImageMedian3D::setKernelSize(int size0, int size1, int size2)
{
    return d->imageMedian3D->SetKernelSize(size0, size1, size2);
}

/**
 \todo
*/
int CVtkImageMedian3D::numberOfElements() const
{
    return d->imageMedian3D->GetNumberOfElements();
}


bool CVtkImageMedian3D::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::hasInput(path);
}

bool CVtkImageMedian3D::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::setInput(path, inputData);
}

bool CVtkImageMedian3D::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkThreadedImageAlgorithm::removeInput(path, inputData);
}

bool CVtkImageMedian3D::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkThreadedImageAlgorithm::fetchOutput(path, outputData);
}

bool CVtkImageMedian3D::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkImageMedian3D::propertyCount()
{
    return 0;
}

QString CVtkImageMedian3D::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkImageMedian3D::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageMedian3D::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkImageMedian3D::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkImageMedian3D::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkImageMedian3D::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkImageMedian3D::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkImageMedian3D::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkImageMedian3D::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkImageMedian3D::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkImageMedian3D::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkImageMedian3D::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkImageMedian3D::setVtkObject(vtkObject* ptr)
{
    d->imageMedian3D = dynamic_cast<vtkImageMedian3D*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->imageMedian3D);
    CVtkThreadedImageAlgorithm::setVtkObject(ptr);
}





