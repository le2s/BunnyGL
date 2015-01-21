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

#ifndef CPP_CODE_GENERATOR_H
#define CPP_CODE_GENERATOR_H

#include <QObject>
#include <GCF/IAboutData>
#include "ICodeGenerator.h"
#include "CppCodeGeneratorInterfaces.h"

struct CppCodeGeneratorData;
class CppCodeGenerator : public QObject, virtual public ICodeGenerator,
                         virtual IAboutData, virtual ICppPropertyGenerator,
                         virtual ICppParamGeneratorRegistry,
                         virtual ICppConnectionCodeGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICodeGenerator IAboutData ICppPropertyGenerator)
    Q_INTERFACES(ICppParamGeneratorRegistry ICppConnectionCodeGenerator)

public:
    CppCodeGenerator(QObject* parent=0);
    ~CppCodeGenerator();

    // IContainer implementation
    QObject* containerObject();

    // ICodeGenerator implementation
    QString name() const;
    QIcon icon() const;
    QString description() const;
    bool generateCode(const QDir& dir, IVisNetwork* network, ICodeGeneratorManager* codeGenMan);
    bool generateCode(QMap<QString,QString>& code, IVisNetwork* network, ICodeGeneratorManager* codeGenMan);
    void cancelCodeGeneration();

signals: // ICodeGenerator signals
    void progressValue(int percent);
    void progressMessage(const QString& msg);

public:
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

    // ICppPropertyGenerator implementation
    bool canGenerateProperty(IVisSystemNode* node,
                     IVisSystemNodeDesc* nodeDesc,
                     const QString& propName,
                     const QStringList& params,
                     const QVariant& value);
    bool generateProperty(IVisSystemNode* node,
                 IVisSystemNodeDesc* nodeDesc,
                 const QString& propName,
                 const QStringList& params,
                 const QVariant& value,
                 QTextStream& ts, int tab);

    // ICppParamGeneratorRegistry implementation
    QList<ICppParamGenerator*> paramGenerators() const;

    // ICppConnectionCodeGenerator implementation
    bool canGenerateConnectionCode(IVisNetworkConnection* con);
    bool generateConnectionCode(IVisNetworkConnection* con,
                                QTextStream& ts, int tab);

private:
    friend struct CppCodeGeneratorData;
    CppCodeGeneratorData* d;
};

#endif
