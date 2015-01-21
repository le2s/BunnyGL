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
#ifndef MPRVIEWER_COMPONENT_H
#define MPRVIEWER_COMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>
#include "IVisSystemNodeFactory.h"

struct MPRViewerComponentData;
class MPRViewerComponent : public GCF::AbstractComponent,
                                 virtual public IVisSystemNodeFactory,
                                 virtual public IAboutData
{
    Q_OBJECT
    Q_INTERFACES(IVisSystemNodeFactory IAboutData)
    GCF_DECLARE_COMPONENT(MPRViewerComponent)

public:
    static MPRViewerComponent & instance();
    ~MPRViewerComponent();

    static QIcon nodeIcon();

    // IContainer implementation
    QObject* containerObject();

    // IVisSystemNodeFactory implementation
    QStringList nodeClassNameList();
    IVisSystemNodeDesc* nodeDesc(QString nodeClassName);
    bool canCreate(QString nodeClassName);
    IVisSystemNode* createNode(QString nodeClassName);
    void  deleteNode(IVisSystemNode* node);

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

protected:
    MPRViewerComponent();
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void componentDestroyed(GCF::AbstractComponent* comp);

private slots:
    void slotNodeContextMenuEvent(IVisSystemNode* node, QMenu* menu, Qt::KeyboardModifiers modifiers);

private:
    MPRViewerComponentData* d;
};

#endif

