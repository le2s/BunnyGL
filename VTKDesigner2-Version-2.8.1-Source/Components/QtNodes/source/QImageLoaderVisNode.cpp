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

#include "QImageLoaderVisNode.h"
#include "QtNodesComponent.h"

#include <QFileInfo>

DEFINE_VIS_NODE(QImageLoaderVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("QtNodes");
    pDesc->setNodeClassName("QImageLoader");
    pDesc->setNodeClassDescription("QImageLoader");
    pDesc->setNodeIcon( QtNodesComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "ImageOutput",                              // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "QImage",                                   // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct QImageLoaderVisNodeData
{
    QImage image;
    QString fileName;
    int maxWidth;
    int maxHeight;
};

QImageLoaderVisNode::QImageLoaderVisNode()
{
    QImageLoaderVisNode::InitializeNodeDesc();
    d = new QImageLoaderVisNodeData;

    d->maxWidth = 256;
    d->maxHeight = 256;
}

QImageLoaderVisNode::~QImageLoaderVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();


    delete d;
}

void QImageLoaderVisNode::setFileName(const QString& fileName)
{
    if(d->fileName == fileName)
        return;

    d->fileName = fileName;

    if( QFile::exists(fileName) )
        this->loadImageFile();
}

QString QImageLoaderVisNode::fileName() const
{
    return d->fileName;
}

void QImageLoaderVisNode::setMaxWidth(int width)
{
    if(d->maxWidth == width)
        return;

    d->maxWidth = width;
    this->loadImageFile();
}

int QImageLoaderVisNode::maxWidth() const
{
    return d->maxWidth;
}

void QImageLoaderVisNode::setMaxHeight(int height)
{
    if(d->maxHeight == height)
        return;

    d->maxHeight = height;
    this->loadImageFile();
}

int QImageLoaderVisNode::maxHeight() const
{
    return d->maxHeight;
}

bool QImageLoaderVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool QImageLoaderVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool QImageLoaderVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool QImageLoaderVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ImageOutput")
    {
        *outputData = this;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool QImageLoaderVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "ImageOutput")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int QImageLoaderVisNode::propertyCount()
{
    return 0;
}

QString QImageLoaderVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString QImageLoaderVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QImageLoaderVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void QImageLoaderVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool QImageLoaderVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* QImageLoaderVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void QImageLoaderVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant QImageLoaderVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void QImageLoaderVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString QImageLoaderVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QImageLoaderVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool QImageLoaderVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void QImageLoaderVisNode::loadImageFile()
{
    QImage image(d->fileName);

    d->image = image.scaled(d->maxWidth, d->maxHeight, Qt::KeepAspectRatio);
    emit QImageChanged();
}

GCF_DEFINE_QUERY_TABLE(QImageLoaderVisNode)
    GCF_IMPLEMENTS(IQImageIOData)
GCF_END_QUERY_TABLE(QImageLoaderVisNode)

void* QImageLoaderVisNode::dataPointer()
{
    return &d->image;
}

QString QImageLoaderVisNode::dataTypeName()
{
    return "QImage";
}

QImage QImageLoaderVisNode::getQImage()
{
    return d->image;
}

QObject* QImageLoaderVisNode::QImageChangedSignalSender()
{
    return this;
}

