/****************************************************************************
**
** Copyright (C) My Organization
**
** Use of this file is limited according to the terms specified by
** My Organization.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef VRMLEXPORTERCOMPONENT_H
#define VRMLEXPORTERCOMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>

class IVisSystemNode;
class QMenu;
class vtkRenderWindow;

struct VRMLExporterComponentData;
class VRMLExporterComponent : public GCF::AbstractComponent, virtual public IAboutData
{
    Q_OBJECT
    Q_INTERFACES(IAboutData)
    GCF_DECLARE_COMPONENT(VRMLExporterComponent)

public:
    static VRMLExporterComponent & instance();
    ~VRMLExporterComponent();

protected:
    VRMLExporterComponent();
    QAction* fetchAction(const QString& completeName) const;
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void initializeComponent();
    void creationAnnounced();
    void finalizeComponent();

public:
    // IContainer implementation
    QObject* containerObject();
    
    // IAboutData implementation
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
    void slotNodeAdded(IVisSystemNode* node);
    void slotNodeRemoved(IVisSystemNode* node);
    void slotNodeContextMenuEvent(IVisSystemNode* node, QMenu* menu);
    void exportVRML();
    void exportVRML2();

private:
    void exportVRML(vtkRenderWindow* renWin);

private:
    VRMLExporterComponentData* d;
};

#endif

