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

#include "CDataFileLoaderComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>
#include <GCF/ComponentSettings>

#include "IDataFileLoader.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNodeFactoryRegistry.h"

#include "ui_CDFLLoadOptionsDialog.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QApplication>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>

#include "CVTKDataFileLoader.h"
#include "CImageFileLoader.h"
#include "CPDBFileLoader.h"
#include "CPlot3DFileLoader.h"
#include "CDICOMFileLoader.h"
#include "CTerrainImageFileLoader.h"
#include "CVRMLFileLoader.h"
#include "CSTLFileLoader.h"

class BusyCursor
{
public:
    BusyCursor() {
        QApplication::setOverrideCursor( Qt::BusyCursor );
    }

    ~BusyCursor() {
        QApplication::restoreOverrideCursor();
    }
};

struct CDataFileLoaderComponentData
{
    CDataFileLoaderComponentData() : visNetwork(0), visNetworkCanvas(0),
        nodeFactoryReg(0) { }

    QList<IDataFileLoader*> dflList;

    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;
    QString lastDir;
};

GCF_DEFINE_COMPONENT(CDataFileLoaderComponent)

CDataFileLoaderComponent& CDataFileLoaderComponent::instance()
{
    static CDataFileLoaderComponent* theInstance = GCF_CREATE_COMPONENT(CDataFileLoaderComponent);
    return *theInstance;
}

CDataFileLoaderComponent::CDataFileLoaderComponent()
{
    d = new CDataFileLoaderComponentData;
}

CDataFileLoaderComponent::~CDataFileLoaderComponent()
{
    delete d;
}


void CDataFileLoaderComponent::setDefaultDataFilePath(QString path)
{
    d->lastDir = path;
}

QString CDataFileLoaderComponent::defaultDataFilePath() const
{
    return d->lastDir;
}

void CDataFileLoaderComponent::initializeComponent()
{
    // Do Nothing
}

void CDataFileLoaderComponent::finalizeComponent()
{
    settings()["DefaultDataFilePath"] = d->lastDir;
}

QObject* CDataFileLoaderComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    CDataFileLoaderComponent* that = const_cast<CDataFileLoaderComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* CDataFileLoaderComponent::fetchWidget(const QString& completeName) const
{
    Q_UNUSED(completeName);
    return 0;
}

void CDataFileLoaderComponent::creationAnnounced()
{
    if(!settings().isEmpty())
        d->lastDir = settings()["DefaultDataFilePath"].toString();

    QString completeName;
    GCF::AbstractComponent* comp = 0;

    {
        QObject* obj = findObject("IVisNetwork", completeName, &comp);
        if(obj)
        {
            d->visNetwork = qobject_cast<IVisNetwork*>(obj);
            GCF::AbstractComponent::addDependency(this, comp);
        }
    }

    {
        QObject* obj = findObject("IVisNetworkCanvas", completeName, &comp);
        if(obj)
        {
            d->visNetworkCanvas = qobject_cast<IVisNetworkCanvas*>(obj);
            GCF::AbstractComponent::addDependency(this, comp);
        }
    }

    {
        QObject* obj = findObject("IVisSystemNodeFactoryRegistry", completeName, &comp);
        if(obj)
        {
            d->nodeFactoryReg = qobject_cast<IVisSystemNodeFactoryRegistry*>(obj);
            GCF::AbstractComponent::addDependency(this, comp);
        }
    }

    registerDataFileLoader(new CVtkDataFileLoader(this));
    registerDataFileLoader(new CTerrainImageFileLoader(this));
    registerDataFileLoader(new CImageFileLoader(this));
    registerDataFileLoader(new CPDBFileLoader(this));
    registerDataFileLoader(new CPlot3DFileLoader(this));
    registerDataFileLoader(new CDICOMFileLoader(this));
    registerDataFileLoader(new CVRMLFileLoader(this));
    registerDataFileLoader(new CSTLFileLoader(this));
}

void CDataFileLoaderComponent::loadDataFile()
{
    // First create a filter for the file dialog based on the extensions that
    // this component can handle.
    QStringList exts;
    for(int i=0; i<d->dflList.count(); i++)
    {
        IDataFileLoader* dfl = d->dflList[i];
        exts += dfl->handledExtensions();
    }
    exts.sort();

    QString filter;
    QString lastExt;
    for(int i=0; i<exts.count(); i++)
    {
        QString ext = exts[i];
        if(ext == lastExt)
            continue;

        lastExt = ext;
        if(i)
            filter += QString(" *.%1").arg(ext);
        else
            filter = QString("*.%1").arg(ext);
    }

    // Now ask the user to select a file to load.
    if(d->lastDir.isEmpty())
        d->lastDir = QDir::homePath();
    QString fileName = QFileDialog::getOpenFileName(
                            GCF::AbstractComponent::mainWindow(),
                            "Select a data-file to load",
                            d->lastDir,
                            filter);
    if(fileName.isEmpty())
        return;

    // Now store the last opened directory.
    QFileInfo fi(fileName);
    d->lastDir = fi.absolutePath();

    // Now find out if the user wants to load the network into a new canvas, or into the existing canvas.
    bool newCanvas = false;
    if(d->visNetwork->nodeCount())
    {
        QDialog dialog(GCF::AbstractComponent::mainWindow());
        Ui::CDFLLoadOptionsDialog ui;
        ui.setupUi(&dialog);
        if( dialog.exec() == QDialog::Rejected )
            return;
        newCanvas = ui.rdNewCanvas->isChecked();
    }

    // Now perform the actual loading.
    loadDataFile(fileName, newCanvas);
}

void CDataFileLoaderComponent::loadDataFile(const QString& fileName, bool newCanvas)
{
    IDataFileLoader* dfl = this->dataFileLoader(fileName);
    if(!dfl)
    {
        QMessageBox::information(GCF::AbstractComponent::mainWindow(), "Cannot load data file",
            "No data file loader has been register to handle the file\n"
            "\"" + fileName + "\".\n");
        return;
    }

    if(newCanvas)
        d->visNetwork->removeAllNodes();

    BusyCursor busyCursor; 
    bool success = dfl->load(fileName);

    if(success)
        qDebug("Successfully loaded file \"%s\"", qPrintable(fileName));
    else
        qDebug("Error while loading file \"%s\"", qPrintable(fileName));
}

QObject* CDataFileLoaderComponent::containerObject()
{
    return this;
}

void CDataFileLoaderComponent::registerDataFileLoader(IDataFileLoader* loader)
{
    if(!loader)
        return;

    if(d->dflList.contains(loader))
        return;

    d->dflList.append(loader);
    loader->setVisNetwork(d->visNetwork);
    loader->setVisNetworkCanvas(d->visNetworkCanvas);
    loader->setNodeFactoryRegistry(d->nodeFactoryReg);
}

void CDataFileLoaderComponent::unregisterDataFileLoader(IDataFileLoader* loader)
{
    if(!loader)
        return;

    if(!d->dflList.contains(loader))
        return;

    d->dflList.removeAll(loader);
    loader->setVisNetwork(0);
    loader->setVisNetworkCanvas(0);
    loader->setNodeFactoryRegistry(0);
}

int CDataFileLoaderComponent::dataFileLoaderCount()
{
    return d->dflList.count();
}

IDataFileLoader* CDataFileLoaderComponent::dataFileLoader(int index)
{
    if( index < 0 || index >= d->dflList.count() )
        return 0;

    return d->dflList[index];
}

IDataFileLoader* CDataFileLoaderComponent::dataFileLoader(QString fileName)
{
    QFileInfo fi(fileName);
    QString absFileName = fi.absoluteFilePath();

    QString fullExt = fi.completeSuffix().toLower();
    QString ext = fi.suffix().toLower();
    for(int i=0; i<d->dflList.count(); i++)
    {
        IDataFileLoader* dfl = d->dflList[i];
        QStringList exts = dfl->handledExtensions();

        if(!exts.contains(fullExt) && !exts.contains(ext))
            continue;

        if( !dfl->canLoad(absFileName) )
            continue;

        return dfl;
    }

    return 0;
}

QString CDataFileLoaderComponent::productName() const
{
    return "Data File Loader Component (Beta)";
}

QString CDataFileLoaderComponent::organization() const
{
    return "VCreate Logic (P) Ltd";
}

QImage CDataFileLoaderComponent::programLogo() const
{
    return QImage();
}

QString CDataFileLoaderComponent::version() const
{
    return "1.0 - (Beta)";
}

QString CDataFileLoaderComponent::shortDescription() const
{
    return "This component helps you to directly load data files, instead of having to manually "
           "construct pipelines to load them";
}

QString CDataFileLoaderComponent::homepage() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner";
}

QString CDataFileLoaderComponent::bugAddress() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner/feedback/bugs";
}

const QList<GCF::AboutPerson> CDataFileLoaderComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    if(retList.count() == 0)
    {
        retList << GCF::AboutPerson("Prashanth N Udupa", "prashanth@vcreatelogic.com", "http://www.vcreatelogic.com", "Founder, Vision, Design, Development");
    }
    return retList;
}

const QList<GCF::AboutPerson> CDataFileLoaderComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString CDataFileLoaderComponent::license() const
{
    return "Dual License: GPL Version 2 for FOSS development, Commercial for others.";
}

QString CDataFileLoaderComponent::copyrightStatement() const
{
    return QString();
}

GCF_EXPORT_COMPONENT_PLUGIN(CDataFileLoaderComponent)

