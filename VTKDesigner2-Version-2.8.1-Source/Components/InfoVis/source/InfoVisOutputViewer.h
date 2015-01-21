/****************************************************************************
**
** Copyright (C) UNO and VCreate Logic
**
** Use of this file is limited according to the terms specified by
** UNO and VCreate Logic.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef INFO_VIS_OUTPUT_VIEWER_H
#define INFO_VIS_OUTPUT_VIEWER_H

#include <QWidget>

class IVisSystemNode;

struct InfoVisOutputViewerData;
class InfoVisOutputViewer : public QWidget
{
    Q_OBJECT

public:
    InfoVisOutputViewer(QWidget* parent=0);
    ~InfoVisOutputViewer();

    void addViewNode(IVisSystemNode* node, QWidget* widget);
    void removeViewNode(IVisSystemNode* node);
    int viewNodeCount() const;
    IVisSystemNode* viewNodeAt(int index) const;
    QWidget* viewWidgetAt(int index) const;

private slots:
    void slotCurrentRowChanged(int index);
    void slotNodeAdded(IVisSystemNode* node);
    void slotNodeWidgetDestroyed(QObject* obj);
    void slotNodeSelected(IVisSystemNode* node);
    void slotNodeNameChanged();

private:
    InfoVisOutputViewerData* d;
};

#endif
