/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
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

#ifndef C_PIPELINE_PROGRESS_MONITOR_H
#define C_PIPELINE_PROGRESS_MONITOR_H

#include <QWidget>

class IVisNetwork;
class IVisNetworkCanvas;
class IVisSystemNode;
class vtkAlgorithm;
class QTreeWidgetItem;

struct CPipelineProgressMonitorData;
class CPipelineProgressMonitor : public QWidget
{
    Q_OBJECT

public:
    CPipelineProgressMonitor(QWidget* parent=0);
    ~CPipelineProgressMonitor();

    void setVisNetwork(IVisNetwork* network);
    IVisNetwork* visNetwork() const;

    void setVisNetworkCanvas(IVisNetworkCanvas* canvas);
    IVisNetworkCanvas* visNetworkCanvas() const;

protected slots:
    void on_nodeAdded(IVisSystemNode* node);
    void on_nodeRemoved(IVisSystemNode* node);
    void on_nodeClickedEvent(IVisSystemNode* node);
    void on_nodeNameChanged();
    void on_progressTree_currentItemChanged(QTreeWidgetItem* item, QTreeWidgetItem* old);

protected:
    void progressEvent(vtkAlgorithm* alg, QString msg, int percent);

private:
    friend class CProgressMonitor;
    CPipelineProgressMonitorData* d;
};

#endif
