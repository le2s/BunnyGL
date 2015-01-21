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

#ifndef I_CODE_GENERATOR_H
#define I_CODE_GENERATOR_H

#include <GCF/IContainer>
#include <QIcon>
#include <QMap>

class QDir;
class IVisNetwork;
class ICodeGeneratorManager;
class ICodeGenerator : virtual public IContainer
{
public:
    virtual QString name() const = 0;
    virtual QIcon icon() const = 0;
    virtual QString description() const = 0;

    virtual bool generateCode(const QDir& dir, IVisNetwork* network, ICodeGeneratorManager* codeGenMan) = 0;
    virtual bool generateCode(QMap<QString,QString>& code, IVisNetwork* network, ICodeGeneratorManager* codeGenMan) = 0;
    virtual void cancelCodeGeneration() = 0;

    // must be implemented as signals
    virtual void progressValue(int percent) = 0;
    virtual void progressMessage(const QString& msg) = 0;
};
Q_DECLARE_INTERFACE(ICodeGenerator, "com.vcreatelogic.ICodeGenerator/1.0");

#endif
