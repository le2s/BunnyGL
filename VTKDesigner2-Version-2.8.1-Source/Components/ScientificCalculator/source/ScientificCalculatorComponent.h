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

#ifndef SCIENTIFICCALCULATORCOMPONENT_H
#define SCIENTIFICCALCULATORCOMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>

struct ScientificCalculatorComponentData;
class ScientificCalculatorComponent : public GCF::AbstractComponent, virtual public IAboutData
{
    Q_OBJECT
    Q_INTERFACES(IAboutData)
    GCF_DECLARE_COMPONENT(ScientificCalculatorComponent)

public:
    static ScientificCalculatorComponent & instance();
    ~ScientificCalculatorComponent();

protected:
    ScientificCalculatorComponent();
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

private:
    ScientificCalculatorComponentData* d;
};

#endif

