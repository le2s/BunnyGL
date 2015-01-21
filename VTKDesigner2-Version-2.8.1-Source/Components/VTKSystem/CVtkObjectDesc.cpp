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

#include "CVtkObjectDesc.h"
#include "CVtkObjectConnectionPath.h"
#include "CVtkObject.h"
#include "vtkVersion.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QPixmap>
#include <QBitmap>

struct CVtkObjectPropertyInfo
{
    QString SetMethod;
    QString GetMethod;
    QStringList SetMethodParams;
    QStringList GetMethodParams;
    QString SetMethodReturnType;
    QString GetMethodReturnType;
};

struct CVtkObjectDescData
{
    CVtkObjectDescData() : parentDesc(0), metaObject(0), createFunction(0), pathOffset(0) { }

    QString category;
    QString className;
    QString desc;
    CVtkObjectDesc* parentDesc;
    const QMetaObject* metaObject;
    QList<CVtkObjectConnectionPath*> pathList;
    QStringList commandNames;
    QStringList propertyNames;
    QFont font;
    QIcon icon;
    QColor color;
    CVtkObjectCreateFunction createFunction;
    GCF::DataStore nodeDescData;
    QMap<QString, QString> commandFnMap;
    QMap<QString, CVtkObjectPropertyInfo> propertyInfoMap;
    int pathOffset;
};

CVtkObjectDesc::CVtkObjectDesc()
{
    d = new CVtkObjectDescData;

    d->color= Qt::white;
    d->font = QApplication::font();

    static QIcon icon(":/VTKSystem/vtk.png");
    d->icon = icon;
}

CVtkObjectDesc::~CVtkObjectDesc()
{
    qDeleteAll(d->pathList);
    delete d;
}

void CVtkObjectDesc::setNodeClassCategory(QString category)
{
    d->category = category;
}

void CVtkObjectDesc::setNodeClassName(QString className)
{
    d->className = className;
}

void CVtkObjectDesc::setNodeClassDescription(QString desc)
{
    d->desc = desc;
}

void CVtkObjectDesc::setParentNodeDesc(CVtkObjectDesc* desc)
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

void CVtkObjectDesc::setQMetaObject(const QMetaObject* qmo)
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

void CVtkObjectDesc::addConnectionPath(CVtkObjectConnectionPath* path)
{
    if(d->pathList.contains(path))
        return;

    path->setPathIndex(d->pathOffset + d->pathList.count());
    d->pathList.append(path);
}

void CVtkObjectDesc::removeConnectionPath(CVtkObjectConnectionPath* path)
{
    d->pathList.removeAll(path);
    for(int i=0; i<d->pathList.count(); i++)
        d->pathList[i]->setPathIndex(d->pathOffset+i);
}

void CVtkObjectDesc::setConnectionPath(int index, CVtkObjectConnectionPath* path)
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

void CVtkObjectDesc::setCommandFunction(QString command, QString function)
{
    d->commandFnMap[command] = function;
    if(!d->commandNames.contains(command))
    {
        d->commandNames.push_back(command);
        qSort(d->commandNames);
    }
}

void CVtkObjectDesc::setPropertyInfo(QString propName, QString setMethod, QString getMethod,
                         QStringList setMethodParams, QStringList getMethodParams,
                         QString setMethodReturnType, QString getMethodReturnType)
{
    CVtkObjectPropertyInfo info;
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

void CVtkObjectDesc::setNodeFont(QFont font)
{
    d->font = font;
}

void CVtkObjectDesc::setNodeIcon(QIcon icon)
{
    d->icon = icon;
}

void CVtkObjectDesc::setNodeColor(QColor color)
{
    d->color = color;
}

void CVtkObjectDesc::setCreateFunction(CVtkObjectCreateFunction fn)
{
    d->createFunction = fn;
}

CVtkObjectCreateFunction CVtkObjectDesc::createFunction()
{
    return d->createFunction;
}

GCF_BEGIN_QUERY_TABLE(CVtkObjectDesc)
    GCF_IMPLEMENTS(CVtkObjectDesc)
    GCF_IMPLEMENTS(IVisSystemNodeDesc)
GCF_END_QUERY_TABLE(CVtkObjectDesc)

QString CVtkObjectDesc::visSystemName()
{
    return "VTK";
}

QString CVtkObjectDesc::visSystemDescription()
{
    return "Visualization ToolKit from Kitware. (www.vtk.org)";
}

QString CVtkObjectDesc::visSystemVersion()
{
    return QString("VTK Version %1. Source Version %2").arg(vtkVersion::GetVTKBuildVersion()).arg(vtkVersion::GetVTKSourceVersion());
}

QString CVtkObjectDesc::nodeClassCategory()
{
    return d->category;
}

QString CVtkObjectDesc::nodeClassName()
{
    if(!d->className.isEmpty())
        return d->className;
    return d->metaObject ? QString(d->metaObject->className()) : QString();
}

QString CVtkObjectDesc::nodeClassDescription()
{
    return d->desc;
}

IVisSystemNodeDesc* CVtkObjectDesc::parentNodeDesc()
{
    return d->parentDesc;
}

GCF::DataStore CVtkObjectDesc::nodeDescData()
{
    return d->nodeDescData;
}

const QMetaObject* CVtkObjectDesc::qMetaObject()
{
    return d->metaObject;
}

int CVtkObjectDesc::connectionPathCount()
{
    return d->pathOffset + d->pathList.count();
}

IVisSystemNodeConnectionPath* CVtkObjectDesc::connectionPath(int index)
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

QStringList CVtkObjectDesc::commandNames()
{
    if(d->parentDesc)
        return d->commandNames + d->parentDesc->commandNames();
    return d->commandNames;
}

bool CVtkObjectDesc::hasCommandInfo(QString command)
{
    if(d->commandFnMap.contains(command))
        return true;
    if(d->parentDesc)
        return d->parentDesc->hasCommandInfo(command);
    return false;
}

QString    CVtkObjectDesc::commandFunction(QString command)
{
    if(d->commandFnMap.contains(command))
        return d->commandFnMap[command];
    if(d->parentDesc)
        return d->parentDesc->commandFunction(command);
    return QString();
}

QStringList CVtkObjectDesc::propertyNames()
{
    if(d->parentDesc)
        return d->propertyNames + d->parentDesc->propertyNames();
    return d->propertyNames;
}

bool CVtkObjectDesc::hasPropertyInfo(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return true;
    if(d->parentDesc)
        return d->parentDesc->hasPropertyInfo(propName);
    return false;
}

QString    CVtkObjectDesc::propertySetMethod(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].SetMethod;
    if(d->parentDesc)
        return d->parentDesc->propertySetMethod(propName);
    return QString("Set%1").arg(propName);
}

QString    CVtkObjectDesc::propertyGetMethod(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].GetMethod;
    if(d->parentDesc)
        return d->parentDesc->propertyGetMethod(propName);
    return QString("Get%1").arg(propName);
}

QStringList CVtkObjectDesc::propertySetParameters(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].SetMethodParams;
    if(d->parentDesc)
        return d->parentDesc->propertySetParameters(propName);
    static QStringList defaultParams = QStringList() << "int";
    return defaultParams;
}

QStringList CVtkObjectDesc::propertyGetParameters(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].GetMethodParams;
    if(d->parentDesc)
        return d->parentDesc->propertyGetParameters(propName);
    static QStringList defaultParams = QStringList() << "int";
    return defaultParams;
}

QString    CVtkObjectDesc::propertySetReturnType(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].SetMethodReturnType;
    if(d->parentDesc)
        return d->parentDesc->propertySetReturnType(propName);
    return "void";
}

QString    CVtkObjectDesc::propertyGetReturnType(QString propName)
{
    if(d->propertyInfoMap.contains(propName))
        return d->propertyInfoMap[propName].GetMethodReturnType;
    if(d->parentDesc)
        return d->parentDesc->propertyGetReturnType(propName);
    return "int";
}

QFont CVtkObjectDesc::nodeFont()
{
    return d->font;
}

QIcon CVtkObjectDesc::nodeIcon()
{
    return d->icon;
}

QColor CVtkObjectDesc::nodeColor()
{
    return d->color;
}

IVisSystemNode* CVtkObjectDesc::createNode()
{
    if(d->createFunction)
    {
        CVtkObject* vtkNode = d->createFunction();
        IVisSystemNode* node = qobject_cast<IVisSystemNode*>(vtkNode);
        return node;
    }

    return 0;
}

bool CVtkObjectDesc::canCreate()
{
    return d->createFunction != 0;
}

