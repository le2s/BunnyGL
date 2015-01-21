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

#ifndef C_GENERIC_VIS_NODE_DESC_H
#define C_GENERIC_VIS_NODE_DESC_H

#include "IVisSystemNodeDesc.h"
#include "IVisSystemNodeConnectionPath.h"

class CGenericVisNode;
class IVisSystemNode;
class CGenericVisNodeConnectionPath;
typedef QObject* (*CGenericVisNodeCreateFunction)();

struct CGenericVisNodeDescData;
class CGenericVisNodeDesc : virtual public IVisSystemNodeDesc
{
public:
    CGenericVisNodeDesc();
    ~CGenericVisNodeDesc();

    // Methods used to populate the description.
    void setVisSystemName(QString name);
    void setVisSystemDescription(QString desc);
    void setVisSystemVersion(QString version);
    void setNodeClassCategory(QString category);
    void setNodeClassName(QString className);
    void setNodeClassDescription(QString desc);
    void setParentNodeDesc(CGenericVisNodeDesc* desc);
    void setQMetaObject(const QMetaObject* qmo);
    void addConnectionPath(CGenericVisNodeConnectionPath* path);
    void removeConnectionPath(CGenericVisNodeConnectionPath* path);
    void setConnectionPath(int index, CGenericVisNodeConnectionPath* path);
    void setCommandFunction(QString command, QString function);
    void setPropertyInfo(QString propName, QString setMethod, QString getMethod,
                         QStringList setMethodParams, QStringList getMethodParams,
                         QString setMethodReturnType, QString getMethodReturnType);
    void setNodeFont(QFont font);
    void setNodeIcon(QIcon icon);
    void setNodeColor(QColor color);
    void setCreateFunction(CGenericVisNodeCreateFunction fn);
    CGenericVisNodeCreateFunction createFunction();

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
    QString commandFunction(QString command);
    QStringList propertyNames();
    bool hasPropertyInfo(QString propName);
    QString propertySetMethod(QString propName);
    QString propertyGetMethod(QString propName);
    QStringList propertySetParameters(QString propName);
    QStringList propertyGetParameters(QString propName);
    QString propertySetReturnType(QString propName);
    QString propertyGetReturnType(QString propName);
    QFont nodeFont();
    QIcon nodeIcon();
    QColor nodeColor();

    // Custom methods.
    IVisSystemNode* createNode();
    bool canCreate();

private:
    CGenericVisNodeDescData* d;
};

class CGenericVisNodeConnectionPath : virtual public IVisSystemNodeConnectionPath
{
public:
    CGenericVisNodeConnectionPath();
    CGenericVisNodeConnectionPath(QString pName, IVisSystemNodeConnectionPath::PathType pType, QString pdType, int pIndex, bool ami=false);
    ~CGenericVisNodeConnectionPath();

    void setPathName(QString pName);
    void setPathType(IVisSystemNodeConnectionPath::PathType pType);
    void setPathDataType(QString pdType);
    void setPathIndex(int pIndex);
    void setAllowMultipleInputs(bool val);

    // IContainer implementation
    QObject* containerObject();

    // IVisSystemNodeConnectionPath implementation
    QString pathName();
    IVisSystemNodeConnectionPath::PathType pathType();
    QString pathDataType();
    int pathIndex();
    bool allowMultipleInputs();

private:
    QString mPathName;
    IVisSystemNodeConnectionPath::PathType mPathType;
    QString mPathDataType;
    int mPathIndex;
    bool mMultipleInputs;
};

inline CGenericVisNodeConnectionPath::CGenericVisNodeConnectionPath()
{
    mPathIndex = -1;
    mPathType = ControlPath;
    mMultipleInputs = false;
}

inline CGenericVisNodeConnectionPath::CGenericVisNodeConnectionPath(QString pName, IVisSystemNodeConnectionPath::PathType pType, QString pdType, int pIndex, bool ami)
{
    mPathName = pName;
    mPathType = pType;
    mPathDataType = pdType;
    mPathIndex = pIndex;
    mMultipleInputs = ami;
}

inline CGenericVisNodeConnectionPath::~CGenericVisNodeConnectionPath()
{

}

inline void CGenericVisNodeConnectionPath::setPathName(QString pName)
{
    mPathName = pName;
}

inline void CGenericVisNodeConnectionPath::setPathType(IVisSystemNodeConnectionPath::PathType pType)
{
    mPathType = pType;
}

inline void CGenericVisNodeConnectionPath::setPathDataType(QString pdType)
{
    mPathDataType = pdType;
}

inline void CGenericVisNodeConnectionPath::setPathIndex(int pIndex)
{
    mPathIndex = pIndex;
}

inline QObject* CGenericVisNodeConnectionPath::containerObject()
{
    return 0;
}

inline QString CGenericVisNodeConnectionPath::pathName()
{
    return mPathName;
}

inline IVisSystemNodeConnectionPath::PathType CGenericVisNodeConnectionPath::pathType()
{
    return mPathType;
}

inline QString CGenericVisNodeConnectionPath::pathDataType()
{
    return mPathDataType;
}

inline int CGenericVisNodeConnectionPath::pathIndex()
{
    return mPathIndex;
}

inline bool CGenericVisNodeConnectionPath::allowMultipleInputs()
{
    return mMultipleInputs;
}

#define DECLARE_VIS_NODE \
public: \
    static CGenericVisNodeDesc NodeDesc; \
    static void InitializeNodeDesc(); \
    IVisSystemNodeDesc* nodeDesc(); \
private:

#define DEFINE_VIS_NODE(ClassName, SuperClass) \
void ClassName##_InitNodeDesc(CGenericVisNodeDesc* pDesc); \
QObject* ClassName##_CreateFunction() { return new ClassName; } \
CGenericVisNodeDesc ClassName::NodeDesc; \
IVisSystemNodeDesc* ClassName::nodeDesc() { return &(ClassName::NodeDesc); } \
void ClassName::InitializeNodeDesc() \
{ \
    static bool initialized = false; \
    if(initialized) \
        return; \
    SuperClass::InitializeNodeDesc(); \
    ClassName::NodeDesc.setQMetaObject(&(ClassName::staticMetaObject)); \
    ClassName::NodeDesc.setParentNodeDesc(&(SuperClass::NodeDesc)); \
    ClassName::NodeDesc.setCreateFunction(ClassName##_CreateFunction); \
    ClassName##_InitNodeDesc(&(ClassName::NodeDesc)); \
    initialized = true; \
} \
void ClassName##_InitNodeDesc(CGenericVisNodeDesc* pDesc)

#define DEFINE_VIS_NODE1(ClassName) \
void ClassName##_InitNodeDesc(CGenericVisNodeDesc* pDesc); \
QObject* ClassName##_CreateFunction() { return new ClassName; } \
CGenericVisNodeDesc ClassName::NodeDesc; \
IVisSystemNodeDesc* ClassName::nodeDesc() { return &(ClassName::NodeDesc); } \
void ClassName::InitializeNodeDesc() \
{ \
    static bool initialized = false; \
    if(initialized) \
        return; \
    ClassName::NodeDesc.setQMetaObject(&(ClassName::staticMetaObject)); \
    ClassName::NodeDesc.setParentNodeDesc(0); \
    ClassName::NodeDesc.setCreateFunction(ClassName##_CreateFunction); \
    ClassName##_InitNodeDesc(&(ClassName::NodeDesc)); \
    initialized = true; \
} \
void ClassName##_InitNodeDesc(CGenericVisNodeDesc* pDesc)

#define REGISTER_NODE(ClassName, Map) \
ClassName::InitializeNodeDesc(); \
Map[ClassName::NodeDesc.nodeClassName()] = (&ClassName::NodeDesc);


#endif
