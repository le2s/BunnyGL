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

#ifndef C_VTK_OBJECT_H
#define C_VTK_OBJECT_H

#include <QObject>
#include "IVisSystemNode.h"
#include "IVtkObjectWrapper.h"
#include "IVtkObjectWrapperModifier.h"
#include "CVtkObjectDesc.h"
#include "CVtkObjectConnectionPath.h"
#include "vtkCommand.h"

class vtkObject;

struct CVtkObjectData;
class CVtkObject : public QObject, 
                   virtual public IVisSystemNode,
                   virtual public IVtkObjectWrapper,
                   virtual public IVtkObjectWrapperModifier
{
    Q_OBJECT
    Q_INTERFACES(IVisSystemNode IVtkObjectWrapper IVtkObjectWrapperModifier)
    Q_PROPERTY(QString NodeName WRITE setNodeName READ nodeName)
    Q_CLASSINFO("SectionName", "/CVtk*/vtk");

public:
    static CVtkObjectDesc ObjectDesc;
    static void InitializeObjectDesc();

    CVtkObject();
    virtual ~CVtkObject();

    virtual void setVtkObject(vtkObject* object); // IVtkObjectWrapperModifier implementation
    vtkObject* getVtkObject();

    void setNodeFactory(IVisSystemNodeFactory* factory);

    // IContainer Implementation
    QObject* containerObject();

    // IVisSystemNode Implementation
    IVisSystemNodeDesc* nodeDesc();
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

    // Timer Event Handler
    virtual void handleTimerControlSignal();

signals:
    void nodeNameChanged();
    void propertyChanged(const QString& name);

signals: // events.
    void OnModified();
    void OnDelete();
    void OnStart();
    void OnEnd();

private slots:
    void on_timeout();

protected: 
    virtual void stopHandlingVtkEvents();
    virtual void handleVtkEvent(unsigned long event, void* callData);

private:
    friend class CVtkObjectCommand;
    CVtkObjectData* d;
};

#define DECLARE_VTK_OBJECT \
public: \
    static CVtkObjectDesc ObjectDesc; \
    static void InitializeObjectDesc(); \
    IVisSystemNodeDesc* nodeDesc(); \
    void setVtkObject(vtkObject* object); \
private:

#define DEFINE_VTK_OBJECT(ClassName, SuperClass, vtkClass) \
void ClassName##_InitObjectDesc(CVtkObjectDesc* pDesc); \
CVtkObject* ClassName##_CreateFunction() { return new ClassName; } \
CVtkObjectDesc ClassName::ObjectDesc; \
IVisSystemNodeDesc* ClassName::nodeDesc() { return &(ClassName::ObjectDesc); } \
void ClassName::InitializeObjectDesc() \
{ \
    static bool initialized = false; \
    if(initialized) \
        return; \
    SuperClass::InitializeObjectDesc(); \
    ClassName::ObjectDesc.setQMetaObject(&(ClassName::staticMetaObject)); \
    ClassName::ObjectDesc.setParentNodeDesc(&(SuperClass::ObjectDesc)); \
    ClassName::ObjectDesc.setCreateFunction(ClassName##_CreateFunction); \
    ClassName##_InitObjectDesc(&(ClassName::ObjectDesc)); \
    initialized = true; \
} \
void ClassName::setVtkObject(vtkObject* object) \
{ \
    if(m_##vtkClass == object) \
        return; \
    if(m_##vtkClass) \
        m_##vtkClass = 0; \
    m_##vtkClass = dynamic_cast<vtkClass*>(object); \
    SuperClass::setVtkObject(object); \
} \
void ClassName##_InitObjectDesc(CVtkObjectDesc* pDesc)

#define DEFINE_VTK_OBJECT2(ClassName, SuperClass) \
void ClassName##_InitObjectDesc(CVtkObjectDesc* pDesc); \
CVtkObject* ClassName##_CreateFunction() { return new ClassName; } \
CVtkObjectDesc ClassName::ObjectDesc; \
IVisSystemNodeDesc* ClassName::nodeDesc() { return &(ClassName::ObjectDesc); } \
void ClassName::InitializeObjectDesc() \
{ \
    static bool initialized = false; \
    if(initialized) \
        return; \
    SuperClass::InitializeObjectDesc(); \
    ClassName::ObjectDesc.setQMetaObject(&(ClassName::staticMetaObject)); \
    ClassName::ObjectDesc.setParentNodeDesc(&(SuperClass::ObjectDesc)); \
    ClassName::ObjectDesc.setCreateFunction(ClassName##_CreateFunction); \
    ClassName##_InitObjectDesc(&(ClassName::ObjectDesc)); \
    initialized = true; \
} \
void ClassName::setVtkObject(vtkObject* object) \
{ \
    SuperClass::setVtkObject(object); \
} \
void ClassName##_InitObjectDesc(CVtkObjectDesc* pDesc)


#endif


