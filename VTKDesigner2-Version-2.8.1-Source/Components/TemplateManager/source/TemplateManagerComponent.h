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

#ifndef TEMPLATEMANAGERCOMPONENT_H
#define TEMPLATEMANAGERCOMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>

struct TemplateManagerComponentData;
class TemplateManagerComponent : public GCF::AbstractComponent, virtual public IAboutData
{
    Q_OBJECT
    Q_INTERFACES(IAboutData)
    Q_PROPERTY(QString TemplatesDirectory READ templatesDirectory WRITE setTemplatesDirectory)
    GCF_DECLARE_COMPONENT(TemplateManagerComponent)

public:
    static TemplateManagerComponent & instance();
    ~TemplateManagerComponent();

    void setTemplatesDirectory(const QString& dir);
    QString templatesDirectory() const;
    
protected:
    TemplateManagerComponent();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();
    void componentDestroyed(GCF::AbstractComponent* comp);

protected slots:
    void slotLoadFromTemplate();
    void slotSaveAsTemplate();
    
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

private:
    TemplateManagerComponentData* d;
};

#endif

