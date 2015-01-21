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

#ifndef CPP_CODE_GENERATOR_P_H
#define CPP_CODE_GENERATOR_P_H

#include "CppCodeGeneratorInterfaces.h"

/* Parameter generators. Implementations of ICppParamGenerator */
class StringParamGenerator : public QObject,
                          virtual public ICppParamGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppParamGenerator)

public:
    QObject* containerObject();

    bool canGenerateParams(const QStringList& params,
                     const QVariant& value);

    bool generateParams(const QStringList& params,
                  const QVariant& value,
                  QTextStream& ts);
};

class BoolParamGenerator : public QObject,
                          virtual public ICppParamGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppParamGenerator)

public:
    QObject* containerObject();

    bool canGenerateParams(const QStringList& params,
                     const QVariant& value);

    bool generateParams(const QStringList& params,
                  const QVariant& value,
                  QTextStream& ts);
};

class IntParamGenerator : public QObject,
                          virtual public ICppParamGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppParamGenerator)

public:
    QObject* containerObject();

    bool canGenerateParams(const QStringList& params,
                     const QVariant& value);

    bool generateParams(const QStringList& params,
                  const QVariant& value,
                  QTextStream& ts);
};

class IntListParamGenerator : public QObject,
                              virtual public ICppParamGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppParamGenerator)

public:
    QObject* containerObject();

    bool canGenerateParams(const QStringList& params,
                     const QVariant& value);

    bool generateParams(const QStringList& params,
                  const QVariant& value,
                  QTextStream& ts);
};

class DoubleParamGenerator : public QObject,
                             virtual public ICppParamGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppParamGenerator)

public:
    QObject* containerObject();

    bool canGenerateParams(const QStringList& params,
                     const QVariant& value);

    bool generateParams(const QStringList& params,
                  const QVariant& value,
                  QTextStream& ts);
};

class DoubleListParamGenerator : public QObject,
                                 virtual public ICppParamGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppParamGenerator)

public:
    QObject* containerObject();

    bool canGenerateParams(const QStringList& params,
                     const QVariant& value);

    bool generateParams(const QStringList& params,
                  const QVariant& value,
                  QTextStream& ts);
};

class ColorParamGenerator : public QObject,
                            virtual public ICppParamGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppParamGenerator)

public:
    QObject* containerObject();

    bool canGenerateParams(const QStringList& params,
                     const QVariant& value);

    bool generateParams(const QStringList& params,
                  const QVariant& value,
                  QTextStream& ts);
};

/* Connection Generators. Implementations of ICppConnectionGetOutputCodeGenerator,
   ICppConnectionSetInputCodeGenerator */

class VtkAlgorithmGetOutputCodeGenerator : public QObject,
                                           virtual public ICppConnectionGetOutputCodeGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppConnectionGetOutputCodeGenerator)

public:
    QObject* containerObject();

    bool canGenerateGetOutputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con);

    bool generateGetOutputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          QTextStream& ts, int tab,
                          QString& outputVariable);
};

class VtkAlgorithmSetInputCodeGenerator : public QObject,
                                          virtual public ICppConnectionSetInputCodeGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppConnectionSetInputCodeGenerator)

public:
    QObject* containerObject();

    bool canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con);
    bool generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab);
};

class VtkObjectGetOutputCodeGenerator : public QObject,
                                       virtual public ICppConnectionGetOutputCodeGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppConnectionGetOutputCodeGenerator)

public:
    QObject* containerObject();

    bool canGenerateGetOutputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con);

    bool generateGetOutputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          QTextStream& ts, int tab,
                          QString& outputVariable);
};

class VtkActorSetInputCodeGenerator : public QObject,
                                          virtual public ICppConnectionSetInputCodeGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppConnectionSetInputCodeGenerator)

public:
    QObject* containerObject();

    bool canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con);
    bool generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab);
};

class VtkRendererSetInputCodeGenerator : public QObject,
                                          virtual public ICppConnectionSetInputCodeGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppConnectionSetInputCodeGenerator)

public:
    QObject* containerObject();

    bool canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con);
    bool generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab);
};

class VtkRenderWindowSetInputCodeGenerator : public QObject,
                                          virtual public ICppConnectionSetInputCodeGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppConnectionSetInputCodeGenerator)

public:
    QObject* containerObject();

    bool canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con);
    bool generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab);
};

class VtkImplicitBooleanSetInputCodeGenerator : public QObject,
                                          virtual public ICppConnectionSetInputCodeGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppConnectionSetInputCodeGenerator)

public:
    QObject* containerObject();

    bool canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con);
    bool generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab);
};

class VtkSampleFunctionSetInputCodeGenerator : public QObject,
                                          virtual public ICppConnectionSetInputCodeGenerator
{
    Q_OBJECT
    Q_INTERFACES(ICppConnectionSetInputCodeGenerator)

public:
    QObject* containerObject();

    bool canGenerateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con);
    bool generateSetInputCode(IVisSystemNode* node,
                          IVisSystemNodeConnectionPath* path,
                          IVisNetworkConnection* con,
                          const QString& inputCode,
                          QTextStream& ts, int tab);
};

#endif


