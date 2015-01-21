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

#include "QWebPageToQImageConverterVisNode.h"
#include "QtNodesComponent.h"

#include <QWebPage>
#include <QWebFrame>
#include <QPainter>
#include <QImage>
#include <QUrl>

DEFINE_VIS_NODE(QWebPageToQImageConverterVisNode, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("QtNodes");
    pDesc->setNodeClassName("QWebPageToQImageConverter");
    pDesc->setNodeClassDescription("QWebPageToQImageConverter");
    pDesc->setNodeIcon( QtNodesComponent::instance().nodeIcon() );

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "QImageOutput",                             // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "QImage",                                   // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct QWebPageToQImageConverterVisNodeData
{
    QWebPage* webPage;
    QString url;
    QImage image;
    int loadProgress;
    int width;
    int height;
};

QWebPageToQImageConverterVisNode::QWebPageToQImageConverterVisNode()
{
    QWebPageToQImageConverterVisNode::InitializeNodeDesc();
    d = new QWebPageToQImageConverterVisNodeData;

    d->width = 128;
    d->height = 128;
    d->webPage = new QWebPage(this);    
    d->webPage->setViewportSize( QSize(d->width, d->height) );
    connect(d->webPage, SIGNAL(loadStarted()), this, SLOT(webPageLoadStarted()));
    connect(d->webPage, SIGNAL(loadFinished(bool)), this, SLOT(webPageLoadFinished(bool)));
    connect(d->webPage, SIGNAL(loadProgress(int)), this, SLOT(webPageLoadProgress(int)));
}

QWebPageToQImageConverterVisNode::~QWebPageToQImageConverterVisNode()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    delete d;
}

void QWebPageToQImageConverterVisNode::setUrl(const QString& url)
{
    d->url = url;
}

QString QWebPageToQImageConverterVisNode::url() const
{
    return d->url;
}

void QWebPageToQImageConverterVisNode::setWidth(int width)
{
    if( width < 0 || d->width == width )
        return;

    d->width = width;
    this->updateImageFromWebPage();
}

int QWebPageToQImageConverterVisNode::width() const
{
    return d->width;
}

void QWebPageToQImageConverterVisNode::setHeight(int height)
{
    if( height < 0 || d->height == height )
        return;

    d->height = height;
    this->updateImageFromWebPage();
}

int QWebPageToQImageConverterVisNode::height() const
{
    return d->height;
}

void QWebPageToQImageConverterVisNode::command_LoadURL()
{
    d->loadProgress = 0;
    d->webPage->mainFrame()->load( QUrl(d->url) );
}

bool QWebPageToQImageConverterVisNode::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool QWebPageToQImageConverterVisNode::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool QWebPageToQImageConverterVisNode::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool QWebPageToQImageConverterVisNode::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "QImageOutput")
    {
        *outputData = this;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool QWebPageToQImageConverterVisNode::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "QImageOutput")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int QWebPageToQImageConverterVisNode::propertyCount()
{
    return 0;
}

QString QWebPageToQImageConverterVisNode::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString QWebPageToQImageConverterVisNode::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QWebPageToQImageConverterVisNode::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void QWebPageToQImageConverterVisNode::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool QWebPageToQImageConverterVisNode::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* QWebPageToQImageConverterVisNode::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void QWebPageToQImageConverterVisNode::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant QWebPageToQImageConverterVisNode::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void QWebPageToQImageConverterVisNode::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString QWebPageToQImageConverterVisNode::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant QWebPageToQImageConverterVisNode::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool QWebPageToQImageConverterVisNode::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

GCF_DEFINE_QUERY_TABLE(QWebPageToQImageConverterVisNode)
    GCF_IMPLEMENTS(IQImageIOData)
GCF_END_QUERY_TABLE(QWebPageToQImageConverterVisNode)

void* QWebPageToQImageConverterVisNode::dataPointer()
{
    return &d->image;
}

QString QWebPageToQImageConverterVisNode::dataTypeName()
{
    return "QImage";
}

QImage QWebPageToQImageConverterVisNode::getQImage()
{
    return d->image;
}

QObject* QWebPageToQImageConverterVisNode::QImageChangedSignalSender()
{
    return this;
}

void QWebPageToQImageConverterVisNode::updateImageFromWebPage()
{
    d->webPage->setViewportSize( QSize(d->width,d->height) );
    d->image = QImage( d->webPage->viewportSize(), QImage::Format_ARGB32 );
    
    QPainter painter(&d->image);
    d->webPage->mainFrame()->render(&painter);
    painter.end();

    d->image.save("C:\\Temp\\WebPage.jpg", "JPG");

    emit QImageChanged();
}

void QWebPageToQImageConverterVisNode::webPageLoadStarted()
{
    emit URLLoadStarted(d->url);
}

void QWebPageToQImageConverterVisNode::webPageLoadProgress(int percent)
{
    if(percent-d->loadProgress > 25)
    {
        d->loadProgress = percent;
        this->updateImageFromWebPage();
    }

    emit URLLoadProgress(d->url, percent);
}

void QWebPageToQImageConverterVisNode::webPageLoadFinished(bool ok)
{
    emit URLLoadFinished(d->url, ok);
    this->updateImageFromWebPage();
}
