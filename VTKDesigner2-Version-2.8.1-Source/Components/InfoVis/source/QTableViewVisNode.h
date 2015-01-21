/****************************************************************************
**
** Copyright (C) UNO and VCreate Logic
**
** Use of this file is limited according to the terms specified by
** UNO and VCreate Logic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTABLEVIEWVISNODE_VIS_NODE_H
#define QTABLEVIEWVISNODE_VIS_NODE_H

#include "UserDefinedDataTypes.h"
#include "QAbstractItemViewVisNode.h"

#ifdef ENABLE_ADVANCED_PROPERTIES
#include "IEditableProperties.h"
#endif

struct QTableViewVisNodeData;
class QTableViewVisNode : public QAbstractItemViewVisNode
#ifdef ENABLE_ADVANCED_PROPERTIES
                       , virtual public IEditableProperties
                       , virtual public IEditablePropertiesExt
#endif
{
    Q_OBJECT
    Q_PROPERTY(bool VerticalHeaderVisible READ isVerticalHeaderVisible WRITE setVerticalHeaderVisible)
    Q_PROPERTY(bool HorizontalHeaderVisible READ isHorizontalHeaderVisible WRITE setHorizontalHeaderVisible)
    Q_PROPERTY(bool GridVisible READ isGridVisible WRITE setGridVisible)
    Q_PROPERTY(bool SortingEnabled READ isSortingEnabled WRITE setSortingEnabled)
    Q_PROPERTY(bool WordWrapEnabled READ isWordWrapEnabled WRITE setWordWrapEnabled)
#ifdef ENABLE_ADVANCED_PROPERTIES
    Q_INTERFACES(IEditableProperties IEditablePropertiesExt)
#endif
    DECLARE_VIS_NODE

public:
    QTableViewVisNode();
    ~QTableViewVisNode();

    void setVerticalHeaderVisible(bool val);
    bool isVerticalHeaderVisible() const;

    void setHorizontalHeaderVisible(bool val);
    bool isHorizontalHeaderVisible() const;

    void setGridVisible(bool val);
    bool isGridVisible() const;

    void setSortingEnabled(bool val);
    bool isSortingEnabled() const;

    void setWordWrapEnabled(bool val);
    bool isWordWrapEnabled() const;

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


private:
    QTableViewVisNodeData* d;
};

#endif

