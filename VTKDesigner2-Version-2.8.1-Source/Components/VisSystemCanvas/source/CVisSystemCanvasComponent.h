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

#ifndef C_VIS_SYSTEM_CANVAS_COMPONENT_H
#define C_VIS_SYSTEM_CANVAS_COMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>

class CVisSystemCanvas;
class IVisSystemNode;
class IVisNetworkCanvas;
class IPropertyEditor;

struct CVisSystemCanvasComponentData;
class CVisSystemCanvasComponent : public GCF::AbstractComponent, virtual public IAboutData
{
    Q_OBJECT
    Q_INTERFACES(IAboutData)
    GCF_DECLARE_COMPONENT(CVisSystemCanvasComponent)

public:
    static CVisSystemCanvasComponent & instance();
    ~CVisSystemCanvasComponent();

    CVisSystemCanvas* canvasObject();
    IVisNetworkCanvas* canvas();

protected:
    CVisSystemCanvasComponent();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void addChildWidget(QWidget* parent, QWidget* child, const QString& hint);
    void removeChildWidget(QWidget* parent, QWidget* child, const QString& hint);
    void componentCreated(AbstractComponent* comp);
    void componentDestroyed(AbstractComponent* comp);
    void creationAnnounced();
    void initializeComponent();
    void finalizeComponent();
    IPropertyEditor* getNewPropertyEditorInstance() const; // for CVisSystemCanvas

public:
    QObject* containerObject();
    QString productName() const;
    QString organization() const;
    QImage programLogo() const;
    QString version() const;
    QString shortDescription() const;
    QString homepage() const;
    QString bugAddress() const;
    const QList<GCF::AboutPerson> authors() const;
    const QList<GCF::AboutPerson> credits() const;
    QString license() const;
    QString copyrightStatement() const;

protected slots:
    void on_workspaceWidgetActivated(QWidget* widget);
    void on_nodeAdded(IVisSystemNode* node);
    void on_nodeRemoved(IVisSystemNode* node);
    void on_nodeNameChanged();
    void on_nodeClickedEvent(IVisSystemNode* node, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
    void on_canvasClickedEvent(IVisNetworkCanvas* canvas, const QPoint pos, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);

private:
    friend class CVisSystemCanvas;
    CVisSystemCanvasComponentData* d;
};

#endif
