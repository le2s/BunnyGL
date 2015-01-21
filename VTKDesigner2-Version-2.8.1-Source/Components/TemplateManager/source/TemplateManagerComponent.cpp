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

#include "TemplateManagerComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>

#include <IProject>
#include <IProjectExt>
#include <QMessageBox>
#include <QMainWindow>
#include <QFileDialog>

GCF_DEFINE_COMPONENT(TemplateManagerComponent)

struct TemplateManagerComponentData
{
    TemplateManagerComponentData() : project(0), projectExt(0), projectComp(0) { }
    
    IProject* project;
    IProjectExt* projectExt;
    GCF::AbstractComponent* projectComp;

    QString templatesDirectory;
};

TemplateManagerComponent & TemplateManagerComponent::instance()
{
    static TemplateManagerComponent* theInstance = GCF_CREATE_COMPONENT(TemplateManagerComponent);
    return *theInstance;
}

TemplateManagerComponent::TemplateManagerComponent()
{
    d = new TemplateManagerComponentData;
}

TemplateManagerComponent::~TemplateManagerComponent()
{
    delete d;
}

void TemplateManagerComponent::setTemplatesDirectory(const QString& dir)
{
    d->templatesDirectory = dir;
}

QString TemplateManagerComponent::templatesDirectory() const
{
    return d->templatesDirectory;
}

QObject* TemplateManagerComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    TemplateManagerComponent* that = const_cast<TemplateManagerComponent*>(this);
    
    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* TemplateManagerComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    return 0;
}

void TemplateManagerComponent::initializeComponent()
{
    // Do nothing.
}

void TemplateManagerComponent::finalizeComponent()
{
    // Do nothing
}

void TemplateManagerComponent::creationAnnounced()
{
    // Find out IProject and IProjectExt interfaces
    QString name;
    QObject* obj = findObject("IProject", name, &d->projectComp);
    if(obj)
        d->project = qobject_cast<IProject*>(obj);

    obj = findObject("IProjectExt", name, &d->projectComp);
    if(obj)
        d->projectExt = qobject_cast<IProjectExt*>(obj);
}

void TemplateManagerComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->projectComp)
    {
        d->projectComp = 0;
        d->project = 0;
    }
}

void TemplateManagerComponent::slotLoadFromTemplate()
{
    if(!d->project || !d->projectExt)
    {
        QMessageBox::information( mainWindow(), "Project Manager not found",
                                  "The project manager component was not found. Please\n"
                                  "write to support@vcreatelogic.com about this.");
        return;
    }

    QString currentProjFile = d->projectExt->currentProjectFileName();
    if(!currentProjFile.isEmpty() || d->project->isModified())
    {
        // Ask the user if he wants to save the current project file.
        QMessageBox::StandardButton res = QMessageBox::question( mainWindow(),
                                                                 "Save current project",
                                                                 "Do you want to save the current project ?",
                                                                 QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

        if(res == QMessageBox::Cancel)
            return;

        if(res == QMessageBox::Yes)
        {
            if(currentProjFile.isEmpty())
                currentProjFile = QFileDialog::getSaveFileName( mainWindow(), "Save project",
                                                                d->projectExt->defaultProjectPath(),
                                                                QString("VTK Designer Project (*.%1)").arg(d->projectExt->projectFileSuffix()) );
            d->project->saveProject( currentProjFile );
        }        
    }

    // Start a new project first.
    d->project->newProject();

    QString fileName = QFileDialog::getOpenFileName(mainWindow(),
                                                    "Select a template to load",
                                                    d->templatesDirectory,
                                                    "VTK Designer Template (*.vwt)");
    if(fileName.isEmpty())
        return;

    // Load the project from the template file.
    d->project->loadProject(fileName); // Note that the project is marked as unmodified at this time
}

void TemplateManagerComponent::slotSaveAsTemplate()
{
    if(!d->project || !d->projectExt)
    {
        QMessageBox::information( mainWindow(), "Project Manager not found",
                                  "The project manager component was not found. Please\n"
                                  "write to support@vcreatelogic.com about this.");
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName( mainWindow(), "Save as template",
                                                     d->templatesDirectory,
                                                     "VTK Designer Template (*.vwt)");
    d->project->saveProject(fileName);
}

QObject* TemplateManagerComponent::containerObject()
{
    return this;
}

QString TemplateManagerComponent::productName() const
{
    return "TemplateManager";
}

QString TemplateManagerComponent::organization() const
{
    return "VCreate Logic Pvt Ltd";
}

QImage TemplateManagerComponent::programLogo() const
{
    return QImage();
}

QString TemplateManagerComponent::version() const
{
    return "1.0";
}

QString TemplateManagerComponent::shortDescription() const
{
    return "Provides TemplateManager";
}

QString TemplateManagerComponent::homepage() const
{
    return "VCreate Logic Pvt Ltd website";
}

QString TemplateManagerComponent::bugAddress() const
{
    return "VCreate Logic Pvt Ltd website";
}

const QList<GCF::AboutPerson> TemplateManagerComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> TemplateManagerComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString TemplateManagerComponent::license() const
{
    return "VCreate Logic Pvt Ltd {{AppName}} License";
}

QString TemplateManagerComponent::copyrightStatement() const
{
    return QString();
}

GCF_EXPORT_COMPONENT_PLUGIN(TemplateManagerComponent)

