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

#ifndef CPP_CODE_GENERATOR_INTERFACES_H
#define CPP_CODE_GENERATOR_INTERFACES_H

#include <GCF/IContainer>
#include <QStringList>
#include <QVariant>
#include <QTextStream>

#include "IVisSystemNode.h"
#include "IVisSystemNodeDesc.h"
#include "IVisNetworkConnection.h"
#include "IVisSystemNodeConnectionPath.h"

class ICppPropertyGenerator : virtual public IContainer
{
public:
    virtual bool canGenerateProperty(IVisSystemNode* node,
                             IVisSystemNodeDesc* nodeDesc,
                             const QString& propName,
                             const QStringList& params,
                             const QVariant& value) = 0;

    virtual bool generateProperty(IVisSystemNode* node,
                             IVisSystemNodeDesc* nodeDesc,
                             const QString& propName,
                             const QStringList& params,
                             const QVariant& value,
                             QTextStream& ts, int tab) = 0;
};
Q_DECLARE_INTERFACE(ICppPropertyGenerator, "com.vcreatelogic.ICppPropertyGenerator/1.0")

class ICppParamGenerator : virtual public IContainer
{
public:
    virtual bool canGenerateParams(const QStringList& params,
                             const QVariant& value) = 0;
    virtual bool generateParams(const QStringList& params,
                          const QVariant& value,
                          QTextStream& ts) = 0;
};
Q_DECLARE_INTERFACE(ICppParamGenerator, "com.vcreatelogic.ICppParamGenerator/1.0")

class ICppParamGeneratorRegistry : virtual public IContainer
{
public:
    virtual QList<ICppParamGenerator*> paramGenerators() const = 0;
};
Q_DECLARE_INTERFACE(ICppParamGeneratorRegistry, "com.vcreatelogic.ICppParamGeneratorRegistry/1.0")

class ICppConnectionGetOutputCodeGenerator : virtual public IContainer
{
public:
    virtual bool canGenerateGetOutputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con) = 0;
    virtual bool generateGetOutputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          QTextStream& ts, int tab,
                          QString& outputVariable) = 0;
};
Q_DECLARE_INTERFACE(ICppConnectionGetOutputCodeGenerator, "com.vcreatelogic.ICppConnectionGetOutputCodeGenerator/1.0")

class ICppConnectionSetInputCodeGenerator : virtual public IContainer
{
public:
    virtual bool canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con) = 0;
    virtual bool generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab) = 0;
};
Q_DECLARE_INTERFACE(ICppConnectionSetInputCodeGenerator, "com.vcreatelogic.ICppConnectionSetInputCodeGenerator/1.0")

class ICppConnectionCodeGenerator : virtual public IContainer
{
public:
    virtual bool canGenerateConnectionCode(IVisNetworkConnection* con) = 0;
    virtual bool generateConnectionCode(IVisNetworkConnection* con,
                                        QTextStream& ts, int tab) = 0;
};
Q_DECLARE_INTERFACE(ICppConnectionCodeGenerator, "com.vcreatelogic.ICppConnectionCodeGenerator/1.0")

#endif

