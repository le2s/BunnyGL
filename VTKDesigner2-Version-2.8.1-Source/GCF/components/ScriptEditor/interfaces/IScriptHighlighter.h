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

#ifndef I_SCRIPT_HIGHLIGHTER_H
#define I_SCRIPT_HIGHLIGHTER_H

#include <GCF/IContainer>

class QSyntaxHighlighter;
class IScriptHighlighter : virtual public IContainer
{
public:
    virtual QSyntaxHighlighter* highlighter() const = 0;
    virtual void setHighlighter(QSyntaxHighlighter* h) = 0;
    virtual void setDefaultHighlighter() = 0;
    virtual QSyntaxHighlighter* newDefaultHighlighter() const = 0;
};

Q_DECLARE_INTERFACE(IScriptHighlighter, "com.vcreatelogic.IScriptHighlighter/1.0");

#endif
