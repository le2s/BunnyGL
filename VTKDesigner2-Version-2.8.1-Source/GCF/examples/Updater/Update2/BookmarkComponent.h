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

#ifndef BOOKMARK_COMPONENT_H
#define BOOKMARK_COMPONENT_H

#include <GCF/AbstractComponent>

struct BookmarkComponentData;
class BookmarkComponent : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(BookmarkComponent)

    public:
        BookmarkComponent& instance();
    ~BookmarkComponent();

protected:
    BookmarkComponent();
    QObject* fetchObject(const QString& completeName) const;
    QAction* fetchAction(const QString& completeName) const;
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();

protected slots:
    void slotShowBookmarkFromQAction();
    void bookmarkCurrentPage();
    void showBookmarksPanel();

private:
    BookmarkComponentData* d;
};

#endif

