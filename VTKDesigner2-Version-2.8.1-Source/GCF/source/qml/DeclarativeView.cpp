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

#include <GCF/DeclarativeView>
#include <GCF/ComponentFactory>
#include <GCF/ObjectIterator>
#include <GCF/ObjectList>
#include <GCF/Application>

#include <QDeclarativeContext>
#include <QDeclarativeEngine>

#include <QMap>
#include <QtDebug>
#include <QBasicTimer>
#include <QFileInfo>
#include <QDir>

///////////////////////////////////////////////////////////////////////////////
// DeclarativeView
///////////////////////////////////////////////////////////////////////////////

namespace GCF
{

struct DeclarativeViewData
{
    QUrl source;
};

}

GCF::DeclarativeView::DeclarativeView(QWidget *parent)
    :QDeclarativeView(parent)
{
    d = new GCF::DeclarativeViewData;
    GCF::DeclarativeEnvironment::instance().addView(this);

    this->setRenderHint(QPainter::Antialiasing, true);
    this->setResizeMode(QDeclarativeView::SizeRootObjectToView);
}

GCF::DeclarativeView::~DeclarativeView()
{
    GCF::DeclarativeEnvironment::instance().removeView(this);
    delete d;
}

void GCF::DeclarativeView::setSourceLater(const QUrl &source)
{
    d->source = source;
    connect( gApp, SIGNAL(allComponentsLoaded()), this, SLOT(onAllComponentsLoaded()) );
}

void GCF::DeclarativeView::setTransparentBackground(bool val)
{
    if(val)
    {
        this->setBackgroundBrush(QColor(0,0,0,0));
        this->viewport()->setAutoFillBackground(false);
    }
    else
    {
        this->setBackgroundBrush(Qt::white);
        this->viewport()->setAutoFillBackground(true);
    }
}

bool GCF::DeclarativeView::isTransparentBackground() const
{
    return !this->viewport()->autoFillBackground();
}

void GCF::DeclarativeView::onAllComponentsLoaded()
{
    if(!d->source.isEmpty())
    {
        this->setSource(d->source);

        if(d->source.scheme() == "file")
        {
            QString fileName = d->source.toLocalFile();
            if( QFileInfo(fileName).absolutePath() == QDir::tempPath() )
                QFile::remove(fileName);
        }
    }
}

void GCF::DeclarativeView::initialzieWithQmlObjects(QDeclarativeEngine* engine)
{
    GCF::DeclarativeEnvironment::instance().initializeContext(engine->rootContext());
}

///////////////////////////////////////////////////////////////////////////////
// DeclarativeEnvironment
///////////////////////////////////////////////////////////////////////////////

namespace GCF
{
    struct DeclarativeEnvironmentData
    {
        QList<GCF::DeclarativeView*> declarativeViews;
        GCF::ObjectMap<QString,QObject> nameObjectMap;
        QList<GCF::DeclarativeViewListener*> listenerList;
    };
}

GCF::DeclarativeEnvironment& GCF::DeclarativeEnvironment::instance()
{
    static GCF::DeclarativeEnvironment theInstance;
    return theInstance;
}

GCF::DeclarativeEnvironment::DeclarativeEnvironment(QObject* parent)
    :QObject(parent)
{
    d = new GCF::DeclarativeEnvironmentData;

    this->addNamedObject("gcf", this);

    QList<GCF::AbstractComponent*> comps = GCF::ComponentFactory::instance().components();
    Q_FOREACH(GCF::AbstractComponent* comp, comps)
        this->slotComponentCreated(comp);

    GCF::ComponentFactory* factory = &GCF::ComponentFactory::instance();
    connect(factory, SIGNAL(componentCreated(GCF::AbstractComponent*)),
            this, SLOT(slotComponentCreated(GCF::AbstractComponent*)));
    connect(factory, SIGNAL(componentDestroyed(GCF::AbstractComponent*)),
            this, SLOT(slotComponentDestroyed(GCF::AbstractComponent*)));
}

GCF::DeclarativeEnvironment::~DeclarativeEnvironment()
{
    delete d;
}

QObject* GCF::DeclarativeEnvironment::findObject(const QString& iface) const
{
    QByteArray ifaceBA = iface.toLatin1();

    GCF::ObjectIterator objIt;
    QObject* obj = objIt.firstObject();
    while(obj)
    {
        if( obj->inherits(ifaceBA.constData() ))
            return obj;

        obj = objIt.nextObject();
    }

    return 0;
}

QObject* GCF::DeclarativeEnvironment::getObject(const QString& completeName) const
{
    return ObjectIterator::object(completeName);
}

void GCF::DeclarativeEnvironment::addView(GCF::DeclarativeView* view)
{
    if(!view)
        return;

    if(d->declarativeViews.contains(view))
        return;

    d->declarativeViews.append(view);
    this->initializeContext(view);
    for(int i=0; i<d->listenerList.count(); i++)
        d->listenerList.at(i)->declarativeViewCreated(view);
}

void GCF::DeclarativeEnvironment::removeView(GCF::DeclarativeView* view)
{
    if(!view)
        return;

    if( !d->declarativeViews.contains(view))
        return;

    d->declarativeViews.removeAll(view);
    for(int i=0; i<d->listenerList.count(); i++)
        d->listenerList.at(i)->declarativeViewDestroyed(view);
}

void GCF::DeclarativeEnvironment::initializeContext(GCF::DeclarativeView* view)
{
    this->initializeContext(view->rootContext());
}

void GCF::DeclarativeEnvironment::initializeContext(QDeclarativeContext* context)
{
    // Register all the available qmlNames to new view
    QList<QString> names = d->nameObjectMap.keys();
    Q_FOREACH(QString name, names)
        context->setContextProperty(name, d->nameObjectMap.value(name));
}

void GCF::DeclarativeEnvironment::addNamedObject(const QString& qmlName, QObject* obj)
{
    if(d->nameObjectMap.contains(qmlName))
    {
        qWarning() << "Can not set two property with the same " << qmlName;
        return;
    }

    QString name = qmlName;
    if(name.at(0).isUpper())
    {
        name.replace(0, 1, name.at(0).toLower());
        qDebug() << "Changing QML name from " << qmlName << " to " << name;
    }

    d->nameObjectMap.map(name, obj);
    Q_FOREACH(DeclarativeView* view, d->declarativeViews)
        view->rootContext()->setContextProperty(name, obj);
}

void GCF::DeclarativeEnvironment::removeNamedObject(const QString& qmlName)
{
    if(qmlName.isEmpty())
        return;

    QString name = qmlName;
    if(name.at(0).isUpper())
        name.replace(0, 1, name.at(0).toLower());

    d->nameObjectMap.unmap(name);

    // We cannot remove QML names from any of the existing declarative views. That
    // is too dangerous!
}

void GCF::DeclarativeEnvironment::slotComponentCreated(GCF::AbstractComponent* comp)
{
    GCF::ComponentGui gui = comp->componentGui();
    for(int i=0; i< gui.nodeCount(); i++)
    {
        GCF::ComponentGuiNode node = gui.node(i);
        if(!node.hasKey("qmlName"))
            continue;

        // Register Qml Name and object
        QString qmlName = node["qmlName"].toString();
        QObject* obj = GCF::ObjectIterator::object(node);
        addNamedObject(qmlName, obj);
    }

    QList<GCF::DeclarativeViewListener*> listenerList = GCF::findObjects<GCF::DeclarativeViewListener*>(comp);
    Q_FOREACH(GCF::DeclarativeViewListener* listener, listenerList)
        d->listenerList.append(listener);
}

void GCF::DeclarativeEnvironment::slotComponentDestroyed(GCF::AbstractComponent* comp)
{
    GCF::ComponentGui gui = comp->componentGui();
    for(int i=0; i< gui.nodeCount(); i++)
    {
        GCF::ComponentGuiNode node = gui.node(i);
        if(!node.hasKey("qmlName"))
            continue;

        QString qmlName = node["qmlName"].toString();
        removeNamedObject(qmlName);
    }

    QList<GCF::DeclarativeViewListener*> listenerList = GCF::findObjects<GCF::DeclarativeViewListener*>(comp);
    Q_FOREACH(GCF::DeclarativeViewListener* listener, listenerList)
        d->listenerList.removeAll(listener);
}

