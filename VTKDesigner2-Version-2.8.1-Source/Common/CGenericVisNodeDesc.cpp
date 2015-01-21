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

#include "CGenericVisNodeDesc.h"
#include "IVisSystemNode.h"
#include "CGenericVisNodeBase.h"

#include <QMetaObject>
#include <QMetaMethod>

struct CGenericVisNodePropertyInfo
{
    QString SetMethod;
    QString GetMethod;
    QStringList SetMethodParams;
    QStringList GetMethodParams;
    QString SetMethodReturnType;
    QString GetMethodReturnType;
};

struct CGenericVisNodeDescData
{
    CGenericVisNodeDescData() : parentDesc(0), metaObject(0), createFunction(0), pathOffset(0) { }

    QString visSystemName;
    QString visSystemDesc;
    QString visSystemVersion;
    QString category;
    QString className;
    QString desc;
    CGenericVisNodeDesc* parentDesc;
    const QMetaObject* metaObject;
    QList<CGenericVisNodeConnectionPath*> pathList;
    QStringList commandNames;
    QStringList propertyNames;
    QFont font;
    QIcon icon;
    QColor color;
    CGenericVisNodeCreateFunction createFunction;
    GCF::DataStore nodeDescData;
    QMap<QString, QString> commandFnMap;
    QMap<QString, CGenericVisNodePropertyInfo> propertyInfoMap;
    int pathOffset;
};

CGenericVisNodeDesc::CGenericVisNodeDesc()
{
    d = new CGenericVisNodeDescData;

    d->color= Qt::white;
    d->font = QApplication::font();
    d->icon = QApplication::windowIcon();
}

CGenericVisNodeDesc::~CGenericVisNodeDesc()
{
    qDeleteAll(d->pathList);
    delete d;
}

void CGenericVisNodeDesc::setVisSystemName(QString name)
{
    d->visSystemName = name;
}

void CGenericVisNodeDesc::setVisSystemDescription(QString desc)
{
    d->visSystemDesc = desc;
}

void CGenericVisNodeDesc::setVisSystemVersion(QString version)
{
    d->visSystemVersion = version;
}

void CGenericVisNodeDesc::setNodeClassCategory(QString category)
{
    d->category = category;
}

void CGenericVisNodeDesc::setNodeClassName(QString className)
{
    d->className = className;
}

void CGenericVisNodeDesc::setNodeClassDescription(QString desc)
{
    d->desc = desc;
}

void CGenericVisNodeDesc::setParentNodeDesc(CGenericVisNodeDesc* desc)
{
    d->parentDesc = desc;
    if(desc)
    {
        d->font = desc->nodeFont();
        d->icon = desc->nodeIcon();
        d->color = desc->nodeColor();
        d->category = desc->nodeClassCategory();
        d->pathOffset = desc->connectionPathCount();
    }
}

void CGenericVisNodeDesc::setQMetaObject(const QMetaObject* qmo)
{
    d->metaObject = qmo;
    if(qmo)
    {
        d->commandNames.clear();
        d->commandFnMap.clear();
        for(int i=qmo->methodOffset(); i<qmo->methodCount(); i++)
        {
            QMetaMethod method = qmo->method(i);
            if(method.methodType() != QMetaMethod::Slot)
                continue;
            if(method.parameterTypes().count())
                continue;
            QString methodName(method.signature());
            if(methodName.startsWith("command_"))
            {
                methodName = methodName.section("command_", 1, 1).section('(', 0, 0);
                d->commandNames.push_back(methodName);
            }
        }
        qSort(d->commandNames);

        d->propertyNames.clear();
        d->propertyInfoMap.clear();
        for(int i=qmo->propertyOffset(); i<qmo->propertyCount(); i++)
        {
            QMetaProperty prop = qmo->property(i);
            d->propertyNames.push_back(prop.name());
        }
        qSort(d->propertyNames);
    }
}

void CGenericVisNodeDesc::addConnectionPath(CGenericVisNodeConnectionPath* path)
{
    if(d->pathList.contains(path))
        return;

    path->setPathIndex(d->pathOffset + d->pathList.count());
    d->pathList.append(path);
}

void CGenericVisNodeDesc::removeConnectionPath(CGenericVisNodeConnectionPath* path)
{
    d->pathList.removeAll(path);
    for(int i=0; i<d->pathList.count(); i++)
        d->pathList[i]->setPathIndex(d->pathOffset+i);
}

void CGenericVisNodeDesc::setConnectionPath(int index, CGenericVisNodeConnectionPath* path)
{
    if(!path)
        return;

    if(index < d->pathOffset)
    {
        if(d->parentDesc)
            d->parentDesc->setConnectionPath(index, path);
    }
    else
    {
        index -= d->pathOffset;
        if(index >= 0 && index < d->pathList.count())
        {
            d->pathList[index] = path;
            path->setPathIndex(index+d->pathOffset);
        }
        else
        {
            d->pathList.append(path);
            path->setPathIndex(d->pathList.count()-1+d->pathOffset);
        }
    }
}

void CGenericVisNodeDesc::setCommandFunction(QString command, QString function)
{
    d->commandFnMap[command] = function;
    if(!d->commandNames.contains(command))
    {
        d->commandNames.push_back(command);
        qSort(d->commandNames);
    }
}

void CGenericVisNodeDesc::setPropertyInfo(QString propName, QString setMethod, QString getMethod,
                         QStringList setMethodParams, QStringList getMethodParams,
                         QString setMethodReturnType, QString getMethodReturnType)
{
    CGenericVisNodePropertyInfo info;
    info.SetMethod = setMethod;
    info.GetMethod = getMethod;
    info.SetMethodParams = setMethodParams;
    info.GetMethodParams = getMethodParams;
    info.SetMethodReturnType = setMethodReturnType;
    info.GetMethodReturnType = getMethodReturnType;
    d->propertyInfoMap[propName] = info;

    if(!d->propertyNames.contains(propName))
    {
        d->propertyNames.push_back(propName);
        qSort(d->propertyNames);
    }
}

void CGenericVisNodeDesc::setNodeFont(QFont font)
{
    d->font = font;
}

void CGenericVisNodeDesc::setNodeIcon(QIcon icon)
{
    d->icon = icon;
}

void CGenericVisNodeDesc::setNodeColor(QColor color)
{
    d->color = color;
}

void CGenericVisNodeDesc::setCreateFunction(CGenericVisNodeCreateFunction fn)
{
    d->createFunction = fn;
}

CGenericVisNodeCreateFunction CGenericVisNodeDesc::createFunction()
{
    return d->createFunction;
}

GCF_BEGIN_QUERY_TABLE(CGenericVisNodeDesc)
    GCF_IMPLEMENTS(CGenericVisNodeDesc)
    GCF_IMPLEMENTS(IVisSystemNodeDesc)
GCF_END_QUERY_TABLE(CGenericVisNodeDesc)

QString CGenericVisNodeDesc::visSystemName()
{
    return d->visSystemName;
}

QString CGenericVisNodeDesc::visSystemDescription()
{
    return d->visSystemDesc;
}

QString CGenericVisNodeDesc::visSystemVersion()
{
    return d->visSystemVersion;
}

QString CGenericVisNodeDesc::nodeClassCategory()
{
    return d->category;
}

QString CGenericVisNodeDesc::nodeClassName()
{
    if(!d->className.isEmpty())
        return d->className;
    return d->metaObject ? QString(d->metaObject->className()) : QString();
}

QString CGenericVisNodeDesc::nodeClassDescription()
{
    return d->desc;
}

IVisSystemNodeDesc* CGenericVisNodeDesc::parentNodeDesc()
{
    return d->parentDesc;
}

GCF::DataStore CGenericVisNodeDesc::nodeDescData()
{
    return d->nodeDescData;
}

const QMetaObject* CGenericVisNodeDesc::qMetaObject()
{
    return d->metaObject;
}

int CGenericVisNodeDesc::connectionPathCount()
{
    return d->pathOffset + d->pathList.count();
}

IVisSystemNodeConnectionPath* CGenericVisNodeDesc::connectionPath(int index)
{
    if(index < d->pathOffset)
    {
        if(d->parentDesc)
            return d->parentDesc->connectionPath(index);
        return 0;
    }

    index -= d->pathOffset;
    if(index >= 0 && index < d->pathList.count())
        return d->pathList[index];
    return 0;
}

QStringList CGenericVisNodeDesc::commandNames()
{
    if(d->parentDesc)
        return d->commandNames + d->parentDesc->commandNames();
    return d->commandNames;
}

bool CGenericVisNodeDesc::hasCommandInfo(QString command)
{
    if(d->commandFnMap.contains(command))
        return true;
    if(d->parentDesc)
        return d->parentDesc->hasCommandInfo(command);
    return false;
}

QString CGenericVisNodeDesc::commandFunction(QString command)
{
    if(d->commandFnMap.contains(command))
        return d->commandFnMap[command];
    if(d->parentDesc)
        return d->parentDesc->commandFunction(command);
    return QString();
}

QStringList CGenericVisNodeDesc::propertyNames()
{
    if(d->parentDesc)
        return d->propertyNames + d->parentDesc->propertyNames();
    return d->propertyNames;
}

bool CGenericVisNodeDesc::hasPropertyInfo(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return true;
    if(d->parentDesc)
        return d->parentDesc->hasPropertyInfo(propName);
    return false;
}

QString CGenericVisNodeDesc::propertySetMethod(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].SetMethod;
    if(d->parentDesc)
        return d->parentDesc->propertySetMethod(propName);
    return QString("Set%1").arg(propName);
}

QString CGenericVisNodeDesc::propertyGetMethod(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].GetMethod;
    if(d->parentDesc)
        return d->parentDesc->propertyGetMethod(propName);
    return QString("Get%1").arg(propName);
}

QStringList CGenericVisNodeDesc::propertySetParameters(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].SetMethodParams;
    if(d->parentDesc)
        return d->parentDesc->propertySetParameters(propName);
    static QStringList defaultParams = QStringList() << "int";
    return defaultParams;
}

QStringList CGenericVisNodeDesc::propertyGetParameters(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].GetMethodParams;
    if(d->parentDesc)
        return d->parentDesc->propertyGetParameters(propName);
    static QStringList defaultParams = QStringList() << "int";
    return defaultParams;
}

QString CGenericVisNodeDesc::propertySetReturnType(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].SetMethodReturnType;
    if(d->parentDesc)
        return d->parentDesc->propertySetReturnType(propName);
    return "void";
}

QString CGenericVisNodeDesc::propertyGetReturnType(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].GetMethodReturnType;
    if(d->parentDesc)
        return d->parentDesc->propertyGetReturnType(propName);
    return "int";
}

QFont CGenericVisNodeDesc::nodeFont()
{
    return d->font;
}

QIcon CGenericVisNodeDesc::nodeIcon()
{
    return d->icon;
}

QColor CGenericVisNodeDesc::nodeColor()
{
    return d->color;
}

IVisSystemNode* CGenericVisNodeDesc::createNode()
{
    if(d->createFunction)
    {
        QObject* nodeObj = d->createFunction();
        IVisSystemNode* node = qobject_cast<IVisSystemNode*>(nodeObj);
        CGenericVisNodeBase* nodeBase = qobject_cast<CGenericVisNodeBase*>(nodeObj);
        if(nodeBase)
            nodeBase->recalculatePathRects();
        return node;
    }

    return 0;
}

bool CGenericVisNodeDesc::canCreate()
{
    return d->createFunction != 0;
}

