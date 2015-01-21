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

#ifndef C_GENERIC_VIS_NODE_BASE_H
#define C_GENERIC_VIS_NODE_BASE_H

#include <QObject>
#include "IVisSystemNode.h"
#include "CGenericVisNodeDesc.h"

struct CGenericVisNodeBaseData;
class CGenericVisNodeBase : public QObject, virtual public IVisSystemNode
{
    Q_OBJECT
    Q_INTERFACES(IVisSystemNode)
    Q_PROPERTY(QString NodeName READ nodeName WRITE setNodeName)
    Q_CLASSINFO("SectionName", "/CVtk*/vtk");
    DECLARE_VIS_NODE

public:
    CGenericVisNodeBase();
    virtual ~CGenericVisNodeBase();

    void setNodeFactory(IVisSystemNodeFactory* factory);

    // IContainer implementation
    QObject* containerObject();

    // IVisSystemNode implementation
    // IVisSystemNodeDesc* nodeDesc(); // declared in DECLARE_VIS_NODE macro
    void setNodeName(QString name);
    QString nodeName();
    IVisSystemNodeFactory* nodeFactory();
    GCF::DataStore nodeData();
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);
    bool fetchControlData(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData** ctrlData);
    bool connectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData);
    bool disconnectControl(IVisSystemNodeConnectionPath* path, IVisSystemNodeControlData* ctrlData);
    bool executeCommand(QString cmdName, QString& result);
    QRectF nodeRect();
    QRectF pathRect(IVisSystemNodeConnectionPath* path);
    void paintNode(QPainter* p, QRectF rect, const QStyleOption& opt);
    void paintConnectionPath(IVisSystemNodeConnectionPath* path, QPainter* p, QRectF rect, const QStyleOption& opt);
    void initializeNode();
    void finalizeNode();
    void recalculatePathRects();

signals:
    void nodeNameChanged();
    void propertyChanged(const QString& name);

private:
    CGenericVisNodeBaseData* d;
};

/*
I know that is is not the right place declare vtkObject* metatype. But it reduces source code size if we do.
*/
class vtkObject;
Q_DECLARE_METATYPE(vtkObject*);

#endif

