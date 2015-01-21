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

#ifndef I_CODE_GENERATOR_MANAGER_H
#define I_CODE_GENERATOR_MANAGER_H

#include <GCF/IContainer>

class ICodeGenerator;
class ICodeGeneratorManager : virtual public IContainer
{
public:
    virtual void registerCodeGenerator(ICodeGenerator* codeGen) = 0;
    virtual void unregisterCodeGenerator(ICodeGenerator* codeGen) = 0;
    virtual int codeGeneratorCount() const = 0;
    virtual ICodeGenerator* codeGenerator(int index) const = 0;
    virtual ICodeGenerator* codeGenerator(const QString& name) const = 0;
};
Q_DECLARE_INTERFACE(ICodeGeneratorManager, "com.vcreatelogic.ICodeGeneratorManager/1.0");

#endif
