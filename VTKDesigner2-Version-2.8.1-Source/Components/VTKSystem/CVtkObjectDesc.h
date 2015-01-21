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

#ifndef C_VTK_OBJECT_DESC_H
#define C_VTK_OBJECT_DESC_H

#include "IVisSystemNodeDesc.h"

class CVtkObject;
class IVisSystemNode;
class CVtkObjectConnectionPath;
typedef CVtkObject* (*CVtkObjectCreateFunction)();

struct CVtkObjectDescData;
class CVtkObjectDesc : virtual public IVisSystemNodeDesc
{
public:
    CVtkObjectDesc();
    ~CVtkObjectDesc();

    // Methods used to populate the description.
    void setNodeClassCategory(QString category);
    void setNodeClassName(QString className);
    void setNodeClassDescription(QString desc);
    void setParentNodeDesc(CVtkObjectDesc* desc);
    void setQMetaObject(const QMetaObject* qmo);
    void addConnectionPath(CVtkObjectConnectionPath* path);
    void removeConnectionPath(CVtkObjectConnectionPath* path);
    void setConnectionPath(int index, CVtkObjectConnectionPath* path);
    void setCommandFunction(QString command, QString function);
    void setPropertyInfo(QString propName, QString setMethod, QString getMethod, 
                         QStringList setMethodParams, QStringList getMethodParams,
                         QString setMethodReturnType, QString getMethodReturnType);
    void setNodeFont(QFont font);
    void setNodeIcon(QIcon icon);
    void setNodeColor(QColor color);
    void setCreateFunction(CVtkObjectCreateFunction fn);
    CVtkObjectCreateFunction createFunction();

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // Implemented from IVisSystemNodeDesc
    QString visSystemName();
    QString visSystemDescription();
    QString visSystemVersion();
    QString nodeClassCategory();
    QString nodeClassName();
    QString nodeClassDescription();
    IVisSystemNodeDesc* parentNodeDesc();
    GCF::DataStore nodeDescData();
    const QMetaObject* qMetaObject();
    int connectionPathCount();
    IVisSystemNodeConnectionPath* connectionPath(int index);
    QStringList commandNames();
    bool hasCommandInfo(QString command);
    QString    commandFunction(QString command);
    QStringList propertyNames();
    bool hasPropertyInfo(QString propName);
    QString    propertySetMethod(QString propName);
    QString    propertyGetMethod(QString propName);
    QStringList propertySetParameters(QString propName);
    QStringList propertyGetParameters(QString propName);
    QString    propertySetReturnType(QString propName);
    QString    propertyGetReturnType(QString propName);
    QFont nodeFont();
    QIcon nodeIcon();
    QColor nodeColor();

    // Custom methods.
    IVisSystemNode* createNode();
    bool canCreate();

private:
    CVtkObjectDescData* d;
};

#endif
