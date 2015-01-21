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

#ifndef CODEGENERATORCOMPONENT_H
#define CODEGENERATORCOMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>
#include "ICodeGeneratorManager.h"

struct CodeGeneratorComponentData;
class CodeGeneratorComponent : public GCF::AbstractComponent, virtual public IAboutData,
                               virtual public ICodeGeneratorManager
{
    Q_OBJECT
    Q_INTERFACES(IAboutData ICodeGeneratorManager)
    GCF_DECLARE_COMPONENT(CodeGeneratorComponent)

public:
    static CodeGeneratorComponent & instance();
    ~CodeGeneratorComponent();

protected:
    CodeGeneratorComponent();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    QMenu* fetchMenu(const QString& completeName) const;
    void initializeComponent();
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

public:
    // ICodeGeneratorManager implementation
    void registerCodeGenerator(ICodeGenerator* codeGen);
    void unregisterCodeGenerator(ICodeGenerator* codeGen);
    int codeGeneratorCount() const;
    ICodeGenerator* codeGenerator(int index) const;
    ICodeGenerator* codeGenerator(const QString& name) const;

protected slots:
    void codeGeneratorActionTriggered(QAction* action);
    void dumpUnsupportPropertiesInfo();

private:
    CodeGeneratorComponentData* d;
};

#endif

