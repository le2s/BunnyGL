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

#include "MPRViewerVisNode.h"
#include "MPRViewerComponent.h"
#include "MPRViewerWidget.h"
#include "IVtkImageDataIOData.h"

DEFINE_VIS_NODE(MPRViewerVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("MPRViewer");
    pDesc->setNodeClassName("MPRViewer");
    pDesc->setNodeClassDescription("MPRViewer");
    pDesc->setNodeIcon( MPRViewerComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "DICOMImageInput",                          // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkImageData",                             // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct MPRViewerVisNodeData
{
    MPRViewerWidget* mprWidget;
};

MPRViewerVisNode::MPRViewerVisNode()
{
    MPRViewerVisNode::InitializeNodeDesc();
    d = new MPRViewerVisNodeData;
    d->mprWidget = new MPRViewerWidget;
}

MPRViewerVisNode::~MPRViewerVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    delete d->mprWidget;
    delete d;
}

MPRViewerWidget* MPRViewerVisNode::viewerWidget() const
{
    return d->mprWidget;
}

bool MPRViewerVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "DICOMImageInput")
        return d->mprWidget->dicomImage() != 0;

    return CGenericVisNodeBase::hasInput(path);
}

bool MPRViewerVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DICOMImageInput")
    {
        IVtkImageDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&data);
        if(success && data)
        {
            vtkImageData* dicomImage = data->getVtkImageData();
            d->mprWidget->setDicomImage(dicomImage);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool MPRViewerVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "DICOMImageInput")
    {
        IVtkImageDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&data);
        if(success && data && data->getVtkImageData() == d->mprWidget->dicomImage())
        {
            d->mprWidget->setDicomImage(0);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool MPRViewerVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool MPRViewerVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int MPRViewerVisNode::propertyCount()
{
    return 0;
}

QString MPRViewerVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString MPRViewerVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant MPRViewerVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void MPRViewerVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool MPRViewerVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* MPRViewerVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void MPRViewerVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant MPRViewerVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void MPRViewerVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString MPRViewerVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant MPRViewerVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool MPRViewerVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void MPRViewerVisNode::showMPRViewer()
{
    d->mprWidget->show();
    d->mprWidget->raise();
}

void MPRViewerVisNode::setNodeName(QString name)
{
    CGenericVisNodeBase::setNodeName(name);
    d->mprWidget->setWindowTitle(name);
}



