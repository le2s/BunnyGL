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

#ifndef QWEBPAGETOQIMAGECONVERTERVISNODE_VIS_NODE_H
#define QWEBPAGETOQIMAGECONVERTERVISNODE_VIS_NODE_H

#include "UserDefinedDataTypes.h"
#include "CGenericVisNodeBase.h"

#include "../interfaces/IQImageIOData.h"

#ifdef ENABLE_ADVANCED_PROPERTIES
#include "IEditableProperties.h"
#endif

struct QWebPageToQImageConverterVisNodeData;
class QWebPageToQImageConverterVisNode : public CGenericVisNodeBase,
                                         virtual public IQImageIOData
#ifdef ENABLE_ADVANCED_PROPERTIES
                       , virtual public IEditableProperties
                       , virtual public IEditablePropertiesExt
#endif
{
    Q_OBJECT
#ifdef ENABLE_ADVANCED_PROPERTIES
    Q_INTERFACES(IEditableProperties IEditablePropertiesExt)
#endif
    Q_PROPERTY(QString URL READ url WRITE setUrl)
    Q_PROPERTY(int Width READ width WRITE setWidth)
    Q_PROPERTY(int Height READ height WRITE setHeight)
    DECLARE_VIS_NODE

public:
    QWebPageToQImageConverterVisNode();
    ~QWebPageToQImageConverterVisNode();

    void setUrl(const QString& url);
    QString url() const;

    void setWidth(int width);
    int width() const;

    void setHeight(int height);
    int height() const;

    Q_INVOKABLE void loadURL() 
    { this->command_LoadURL(); }

protected slots:
    void command_LoadURL();

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

#ifdef ENABLE_ADVANCED_PROPERTIES
public:
    // IEditableProperties implementation
    int propertyCount();
    QString propertyName(int index);
    QString propertyGroup(int index);
    QVariant propertyValue(int index);
    void setPropertyValue(int index, QVariant value);
    bool hasEditor(int index);
    QWidget* createEditor(int index);
    void setEditorValue(int index, QWidget* widget, QVariant value);
    QVariant editorValue(int index, QWidget* widget);
    void connectEditor(int index, QWidget* widget, QObject* receiver, const char* member);
    QString propertyValueString(int index);
    QVariant propertyValueFromString(int index, QString valueStr);

signals:
    void propertyConfigurationChanged();

public:
    // IEditablePropertiesExt implementation
    bool canLoadSaveProperty(int index);
#endif

protected:
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();
    void* dataPointer();
    QString dataTypeName();    
    QImage getQImage();
    QObject* QImageChangedSignalSender();

signals:
    void QImageChanged();
    void URLLoadStarted(const QString& url);
    void URLLoadProgress(const QString& url, int percent);
    void URLLoadFinished(const QString& url, bool ok);

protected slots:
    void updateImageFromWebPage();
    void webPageLoadStarted();
    void webPageLoadProgress(int percent);
    void webPageLoadFinished(bool ok);

private:
    QWebPageToQImageConverterVisNodeData* d;
};

#endif

