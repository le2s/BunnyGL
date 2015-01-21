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

#include "ScientificCalculatorComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>
#include <GCF/ComponentSettings>

#include <IScriptHighlighter>
#include <QSyntaxHighlighter>

#include "CalculatorWidget.h"

GCF_DEFINE_COMPONENT(ScientificCalculatorComponent)

struct ScientificCalculatorComponentData
{
    ScientificCalculatorComponentData() : calculatorWidget(0) { }

    CalculatorWidget* calculatorWidget;
};

ScientificCalculatorComponent & ScientificCalculatorComponent::instance()
{
    static ScientificCalculatorComponent* theInstance = GCF_CREATE_COMPONENT(ScientificCalculatorComponent);
    return *theInstance;
}

ScientificCalculatorComponent::ScientificCalculatorComponent()
{
    d = new ScientificCalculatorComponentData;
}

ScientificCalculatorComponent::~ScientificCalculatorComponent()
{
    delete d;
}

QObject* ScientificCalculatorComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    ScientificCalculatorComponent* that = const_cast<ScientificCalculatorComponent*>(this);
    
    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* ScientificCalculatorComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "calculatorWidget")
    {
        if(!d->calculatorWidget)
            d->calculatorWidget = new CalculatorWidget;

        return d->calculatorWidget;
    }

    return 0;
}

void ScientificCalculatorComponent::initializeComponent()
{
    // Do nothing.
}

void ScientificCalculatorComponent::creationAnnounced()
{
    if(d->calculatorWidget)
    {
        QString history = settings()["history"].toString();
        d->calculatorWidget->setHistory(history);

        IScriptHighlighter* scriptHighlighter 
            = GCF::findObject<IScriptHighlighter*>();
        if(scriptHighlighter)
        {
            QSyntaxHighlighter* h = scriptHighlighter->newDefaultHighlighter();
            d->calculatorWidget->setHighlighter(h);
        }
    }
}

void ScientificCalculatorComponent::finalizeComponent()
{
    if(d->calculatorWidget)
    {
        QString history = d->calculatorWidget->history();
        settings()["history"] = history;
    }
}

QObject* ScientificCalculatorComponent::containerObject()
{
    return this;
}

QString ScientificCalculatorComponent::productName() const
{
    return "ScientificCalculator";
}

QString ScientificCalculatorComponent::organization() const
{
    return "My Organization";
}

QImage ScientificCalculatorComponent::programLogo() const
{
    return QImage();
}

QString ScientificCalculatorComponent::version() const
{
    return "1.0";
}

QString ScientificCalculatorComponent::shortDescription() const
{
    return "Provides ScientificCalculator";
}

QString ScientificCalculatorComponent::homepage() const
{
    return "My Organization website";
}

QString ScientificCalculatorComponent::bugAddress() const
{
    return "My Organization website";
}

const QList<GCF::AboutPerson> ScientificCalculatorComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> ScientificCalculatorComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString ScientificCalculatorComponent::license() const
{
    return "My Organization {{AppName}} License";
}

QString ScientificCalculatorComponent::copyrightStatement() const
{
    return QString();
}

GCF_EXPORT_COMPONENT_PLUGIN(ScientificCalculatorComponent)

