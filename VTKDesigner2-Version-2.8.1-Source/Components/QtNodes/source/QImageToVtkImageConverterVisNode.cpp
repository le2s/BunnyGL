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

#include "QImageToVtkImageConverterVisNode.h"
#include "QtNodesComponent.h"

#include "../interfaces/IQImageIOData.h"
#include "QImageToVtkImageData.h"
#include "CVtkImageData.h"
#include "CVtkLookupTableData.h"

#include <QtDebug>

DEFINE_VIS_NODE(QImageToVtkImageConverterVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("QtNodes");
    pDesc->setNodeClassName("QImageToVtkImageData");
    pDesc->setNodeClassDescription("Converts QImage to vtkImageData");
    pDesc->setNodeIcon( QtNodesComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "ImageOutput",                              // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkImageData",                             // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "LookupTable",                              // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkLookupTable",                           // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "QImageInput",                              // Name of the path
                IVisSystemNodeConnectionPath::InputPath,   // Path type can be OutputPath or InputPath
                "QImage",                                   // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct QImageToVtkImageConverterVisNodeData
{
    QImageToVtkImageConverterVisNodeData() : 
        currentImageProvider(0) { }

    QImageToVtkImageData* qimageToVtkImageData;
    CVtkImageData outputData;
    CVtkLookupTableData lookupTableData;
    IQImageIOData* currentImageProvider;
};

QImageToVtkImageConverterVisNode::QImageToVtkImageConverterVisNode()
{
    QImageToVtkImageConverterVisNode::InitializeNodeDesc();
    d = new QImageToVtkImageConverterVisNodeData;

    d->qimageToVtkImageData = QImageToVtkImageData::New();
}

QImageToVtkImageConverterVisNode::~QImageToVtkImageConverterVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    d->qimageToVtkImageData->Delete();

    delete d;
}

bool QImageToVtkImageConverterVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "QImageInput")
        return d->currentImageProvider != 0;

    return CGenericVisNodeBase::hasInput(path);
}

bool QImageToVtkImageConverterVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "QImageInput")
    {
        IQImageIOData* data = 0;
        bool success = inputData->queryInterface("IQImageIOData", (void**)&data);
        if(success && data)
        {
            QImage image = data->getQImage();
            d->qimageToVtkImageData->SetImage(image);
            connect(data->QImageChangedSignalSender(), SIGNAL(QImageChanged()),
                    this, SLOT(inputQImageChanged()));
            d->currentImageProvider = data;
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool QImageToVtkImageConverterVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "QImageInput")
    {
        IQImageIOData* data = 0;
        bool success = inputData->queryInterface("IQImageIOData", (void**)&data);
        if(success && data)
        {
            d->qimageToVtkImageData->SetImage( QImage() );
            d->currentImageProvider = 0;
            disconnect(data->QImageChangedSignalSender(), 0, this, 0);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool QImageToVtkImageConverterVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ImageOutput")
    {
        d->outputData.setImageData( d->qimageToVtkImageData->GetOutput() );
        *outputData = &d->outputData;
        return true;
    }

    if(path->pathName() == "LookupTable")
    {
        d->lookupTableData.setLookupTable( d->qimageToVtkImageData->GetLookupTable() );
        *outputData = &d->lookupTableData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool QImageToVtkImageConverterVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ImageOutput")
        return true;

    if(path->pathName() == "LookupTable")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int QImageToVtkImageConverterVisNode::propertyCount()
{
    return 0;
}

QString QImageToVtkImageConverterVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString QImageToVtkImageConverterVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QImageToVtkImageConverterVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void QImageToVtkImageConverterVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool QImageToVtkImageConverterVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* QImageToVtkImageConverterVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void QImageToVtkImageConverterVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant QImageToVtkImageConverterVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void QImageToVtkImageConverterVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString QImageToVtkImageConverterVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QImageToVtkImageConverterVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool QImageToVtkImageConverterVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void QImageToVtkImageConverterVisNode::inputQImageChanged()
{
    if(d->currentImageProvider)
    {
        QImage image = d->currentImageProvider->getQImage();
        d->qimageToVtkImageData->SetImage(image);
    }
}


