#include "QMLComponent.h"
#include <QFileInfo>

namespace GCF
{
struct QMLComponentData
{
    QString guixmlFile;
};
}

GCF::QMLComponent::QMLComponent(const QString& componentName, const QString& guixmlFileName, QObject *parent)
    : GCF::AbstractComponent()
{
    d = new GCF::QMLComponentData;

    this->setParent(parent);
    this->setComponentName(componentName);
    d->guixmlFile = guixmlFileName;
}

GCF::QMLComponent::~QMLComponent()
{
    delete d;
}

QString GCF::QMLComponent::qmlComponentPath() const
{
    return QFileInfo(d->guixmlFile).absolutePath();
}

void GCF::QMLComponent::initializeComponent()
{
    this->setGuiXmlFileName(d->guixmlFile);
}

void GCF::QMLComponent::finalizeComponent()
{
    // Do nothing
}

QWidget* GCF::QMLComponent::fetchWidget(const QString &completeName) const
{
    Q_UNUSED(completeName)
    return 0;
}

QObject* GCF::QMLComponent::fetchObject(const QString &completeName) const
{
    if(completeName == QString("%1.Core").arg(this->componentName()) )
        return const_cast<GCF::QMLComponent*>(this);

    return 0;
}

