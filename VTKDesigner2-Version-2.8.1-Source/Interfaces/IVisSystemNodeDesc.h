/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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

#ifndef I_VIS_SYSTEM_NODE_DESC_H
#define I_VIS_SYSTEM_NODE_DESC_H

#include <GCF/IQueryable>
#include <GCF/DataStore>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QList>
#include <QByteArray>

class IVisSystemNodeConnectionPath;

class IVisSystemNodeDesc : public virtual IQueryable
{
public:
    // Basic meta information about the visualization system to which the node belongs.
    virtual QString             visSystemName() = 0;
    virtual QString             visSystemDescription() = 0;
    virtual QString             visSystemVersion() = 0;

    // Basic meta object information
    virtual QString             nodeClassCategory() = 0;
    virtual QString             nodeClassName() = 0;
    virtual QString             nodeClassDescription() = 0;
    virtual IVisSystemNodeDesc* parentNodeDesc() = 0;
    virtual GCF::DataStore          nodeDescData() = 0;

    // IVisSystemNodeDesc subclasses must make use of QMetaObject for
    // properties, signals, events and slots. This makes scripting
    // easy.
    virtual const QMetaObject*  qMetaObject() = 0;

    // Path information
    virtual int                 connectionPathCount() = 0;
    virtual IVisSystemNodeConnectionPath* connectionPath(int index) = 0;

    // Information about commands
    virtual QStringList         commandNames() = 0;
    virtual bool                hasCommandInfo(QString command) = 0;
    virtual QString             commandFunction(QString command) = 0;

    // Information about properties.
    virtual QStringList         propertyNames() = 0;
    virtual bool                hasPropertyInfo(QString propName) = 0;
    virtual QString             propertySetMethod(QString propName) = 0;
    virtual QString             propertyGetMethod(QString propName) = 0;
    virtual QStringList         propertySetParameters(QString propName) = 0;
    virtual QStringList         propertyGetParameters(QString propName) = 0;
    virtual QString             propertySetReturnType(QString propName) = 0;
    virtual QString             propertyGetReturnType(QString propName) = 0;

    // Visual Display Hints and Stuff
    virtual QFont               nodeFont() = 0;
    virtual QIcon               nodeIcon() = 0;
    virtual QColor              nodeColor() = 0;
};

Q_DECLARE_INTERFACE(IVisSystemNodeDesc, "com.vcreatelogic.IVisSystemNodeDesc/1.0")

#endif
