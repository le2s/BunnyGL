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

#ifndef I_SCRIPT_ENGINE_LOAD_FORM_CONTAINER_DATABASE_H
#define I_SCRIPT_ENGINE_LOAD_FORM_CONTAINER_DATABASE_H

#include <GCF/IContainer>

class IScriptEngineLoadFormContainerDatabase : virtual public IContainer
{
public:
    virtual void addLoadFormContainer(QWidget* widget, const QString& completeName) = 0;
    virtual void removeLoadFormContainer(QWidget* widget) = 0;
    virtual void changeLoadFormContainer(QWidget* widget, const QString& completeName) = 0;

    virtual int loadFormContainerCount() const = 0;
    virtual QWidget* loadFormContainerWidgetAt(int index) const = 0;
    virtual QString loadFormContainerNameAt(int index) const = 0;
};
Q_DECLARE_INTERFACE(IScriptEngineLoadFormContainerDatabase, "com.vcreatelogic.IScriptEngineLoadFormContainerDatabase/1.0")

#endif
