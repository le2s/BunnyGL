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

#ifndef I_NEW_UPDATES_HANDLER_H
#define I_NEW_UPDATES_HANDLER_H

#include <GCF/IContainer>
#include <GCF/UpdateInfoList>

class INewUpdatesHandler : virtual public IContainer
{
public:
    virtual bool isAutoDownloadUpdates() const = 0;
    virtual GCF::UpdateInfoList selectUpdates(const GCF::UpdateInfoList& updates) = 0;
};

Q_DECLARE_INTERFACE(INewUpdatesHandler, "com.vcreatelogic.INewUpdatesHandler/1.0");

#endif
