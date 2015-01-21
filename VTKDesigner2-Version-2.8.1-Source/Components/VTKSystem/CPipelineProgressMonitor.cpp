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

#include "CPipelineProgressMonitor.h"
#include "ui_CPipelineProgressMonitor.h"
#include "IVisNetwork.h"
#include "IVisSystemNode.h"
#include "IVisNetworkCanvas.h"
#include "vtkAlgorithm.h"
#include "vtkCommand.h"
#include "vtkMapper.h"
#include <QApplication>
#include <QTime>
#include <QFont>

Q_DECLARE_METATYPE(vtkObject*);

class CProgressMonitor : public vtkCommand
{
public:
    void Execute(vtkObject *caller, unsigned long eventId, void *callData);

    CPipelineProgressMonitor* pipelineProgressMonitor;
    QMap<vtkAlgorithm*, QTime> timeStampMap;
};

struct AlgInfo
{
    AlgInfo() : algorithm(0), item(0) { }

    vtkAlgorithm* algorithm;
    QTreeWidgetItem* item;
};

struct CPipelineProgressMonitorData
{
    CPipelineProgressMonitorData() : visNetwork(0), visNetworkCanvas(0), progressMonitor(0) { }

    Ui::CPipelineProgressMonitor ui;
    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    QMap<IVisSystemNode*, AlgInfo> nodeAlgorithmMap;
    QMap<vtkAlgorithm*, IVisSystemNode*> algorithmNodeMap;
    CProgressMonitor* progressMonitor;

    QFont normalFont;
    QFont progressFont;
};

CPipelineProgressMonitor::CPipelineProgressMonitor(QWidget* parent)
:QWidget(parent)
{
    d = new CPipelineProgressMonitorData;
    d->ui.setupUi(this);
    d->ui.progressTree->setHeaderLabels( QStringList() << "VTK Class" << "Progress" );

    d->progressMonitor = new CProgressMonitor;
    d->progressMonitor->pipelineProgressMonitor = this;

    setMinimumWidth(512);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    d->normalFont = d->ui.progressTree->font();
    d->progressFont = d->normalFont;
    d->progressFont.setBold(true);
}

CPipelineProgressMonitor::~CPipelineProgressMonitor()
{
    delete d->progressMonitor;
    delete d;
}

void CPipelineProgressMonitor::setVisNetwork(IVisNetwork* network)
{
    if(d->visNetwork == network)
        return;

    if(d->visNetwork)
    {
        disconnect(d->visNetwork->containerObject(), 0, this, 0);
        d->ui.progressTree->clear();
        d->ui.txtProgressMessages->clear();
        d->algorithmNodeMap.clear();
        d->nodeAlgorithmMap.clear();
        delete d->progressMonitor;
        d->progressMonitor = new CProgressMonitor;
        d->progressMonitor->pipelineProgressMonitor = this;
    }

    d->visNetwork = network;

    if(d->visNetwork)
    {
        QObject* obj = d->visNetwork->containerObject();
        connect(obj, SIGNAL(nodeAdded(IVisSystemNode*)), this, SLOT(on_nodeAdded(IVisSystemNode*)));
        connect(obj, SIGNAL(nodeRemoved(IVisSystemNode*)), this, SLOT(on_nodeRemoved(IVisSystemNode*)));
    }
}

IVisNetwork* CPipelineProgressMonitor::visNetwork() const
{
    return d->visNetwork;
}

void CPipelineProgressMonitor::setVisNetworkCanvas(IVisNetworkCanvas* canvas)
{
    if( d->visNetworkCanvas == canvas )
        return;

    if(d->visNetworkCanvas)
        disconnect(d->visNetworkCanvas->containerObject(), 0, this, 0);

    d->visNetworkCanvas = canvas;

    if(d->visNetworkCanvas)
    {
        if(d->visNetworkCanvas->visNetwork() != d->visNetwork)
            setVisNetwork(d->visNetworkCanvas->visNetwork());
        connect(d->visNetworkCanvas->containerObject(), 
                SIGNAL(nodeClickedEvent(IVisSystemNode*, const QPoint, Qt::MouseButton, Qt::KeyboardModifiers)),
                this, 
                SLOT(on_nodeClickedEvent(IVisSystemNode*)));
    }
}

IVisNetworkCanvas* CPipelineProgressMonitor::visNetworkCanvas() const
{
    return d->visNetworkCanvas;
}

void CPipelineProgressMonitor::on_nodeAdded(IVisSystemNode* node)
{
    if(!node)
        return;

    if(d->nodeAlgorithmMap.contains(node))
        return;

    GCF::DataStore nodeData = node->nodeData();
    vtkObject* object = nodeData["vtkObject"].value<vtkObject*>();
    vtkAlgorithm* alg = vtkAlgorithm::SafeDownCast(object);
    if(!alg)
        return;

    if(vtkMapper::SafeDownCast(object))
        return;

    connect(node->containerObject(), SIGNAL(nodeNameChanged()), this, SLOT(on_nodeNameChanged()));

    QTreeWidgetItem* item = new QTreeWidgetItem(d->ui.progressTree);
    item->setText(0, node->nodeName());
    item->setText(1, "Not started");
    item->setData(0, Qt::UserRole, qVariantFromValue<IVisSystemNode*>(node));
    d->ui.progressTree->resizeColumnToContents(0);
    
    AlgInfo info;
    info.algorithm = alg;
    info.item = item;
    d->nodeAlgorithmMap[node] = info;
    d->algorithmNodeMap[alg] = node;

    alg->AddObserver(vtkCommand::ProgressEvent, d->progressMonitor);
    alg->AddObserver(vtkCommand::StartEvent, d->progressMonitor);
    alg->AddObserver(vtkCommand::EndEvent, d->progressMonitor);
}

void CPipelineProgressMonitor::on_nodeRemoved(IVisSystemNode* node)
{
    if(!d->nodeAlgorithmMap.contains(node))
        return;

    AlgInfo& info = d->nodeAlgorithmMap[node];
    delete info.item;
    d->nodeAlgorithmMap.remove(node);
    d->algorithmNodeMap.remove(info.algorithm);
    disconnect(node->containerObject(), 0, this, 0);

    // Not needed, because the moment the node is removed, it is deleted.
    // info.algorithm->RemoveObserver(&d->progressMonitor);

    if(!d->ui.progressTree->topLevelItemCount())
        d->ui.txtProgressMessages->clear();
}

void CPipelineProgressMonitor::on_nodeClickedEvent(IVisSystemNode* node)
{
    if(!d->nodeAlgorithmMap.contains(node))
        return;

    AlgInfo& info = d->nodeAlgorithmMap[node];
    d->ui.progressTree->clearSelection();
    info.item->setSelected(true);
    d->ui.progressTree->scrollToItem(info.item);
}

void CPipelineProgressMonitor::on_nodeNameChanged()
{
    QObject* s = sender();
    IVisSystemNode* node = qobject_cast<IVisSystemNode*>(s);

    if(!d->nodeAlgorithmMap.contains(node))
        return;

    AlgInfo& info = d->nodeAlgorithmMap[node];
    info.item->setText(0, node->nodeName());
    d->ui.progressTree->resizeColumnToContents(0);
}

void CPipelineProgressMonitor::on_progressTree_currentItemChanged(QTreeWidgetItem* item, QTreeWidgetItem* old)
{
    if(!item)
        return;

    if(!d->visNetworkCanvas)
        return;

    IVisSystemNode* node = item->data(0, Qt::UserRole).value<IVisSystemNode*>();
    d->visNetworkCanvas->select(node, true);

    node = old ? old->data(0, Qt::UserRole).value<IVisSystemNode*>() : 0;
    if(node)
        d->visNetworkCanvas->select(node, false);
}

void CPipelineProgressMonitor::progressEvent(vtkAlgorithm* alg, QString msg, int percent)
{
    if(!d->algorithmNodeMap.contains(alg))
        return;

    IVisSystemNode* node = d->algorithmNodeMap[alg];
    if(!d->nodeAlgorithmMap.contains(node))
        return;

    AlgInfo& info = d->nodeAlgorithmMap[node];
    if(percent < 100)
    {
        info.item->setText(1, QString("%1").arg(percent));
        info.item->setData(0, Qt::FontRole, d->progressFont);
    }
    else
    {
        info.item->setText(1, msg);
        info.item->setData(0, Qt::FontRole, d->normalFont);
    }
    d->ui.progressTree->scrollToItem(info.item);
    if(msg.isEmpty())
        return;

    QString text = QString("%1: %2").arg(node->nodeName()).arg(msg);
    d->ui.txtProgressMessages->append(text);
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents|QEventLoop::ExcludeSocketNotifiers);
}

void CProgressMonitor::Execute(vtkObject *caller, unsigned long eventId, void *callData)
{
    if(eventId != vtkCommand::ProgressEvent &&
        eventId != vtkCommand::StartEvent &&
        eventId != vtkCommand::EndEvent)
            return;

    vtkAlgorithm* alg = vtkAlgorithm::SafeDownCast(caller);
    if(!alg)
        return;

    int percent = 0; 
    QString msg; 

    switch(eventId)
    {
    case vtkCommand::StartEvent:
        percent = 0;
        msg = "Started";
        this->timeStampMap[alg] = QTime::currentTime();
        QApplication::setOverrideCursor( Qt::WaitCursor );
        break;
    case vtkCommand::EndEvent: {
        int msecs = this->timeStampMap[alg].msecsTo( QTime::currentTime() );
        percent = 100;
        msg = QString("Finished in %1 msecs").arg(msecs);
        this->timeStampMap.remove(alg);
        QApplication::restoreOverrideCursor();
        } break;
    case vtkCommand::ProgressEvent:
        percent = int(alg->GetProgress() * 100.0);
        msg = QString("%1").arg(alg->GetProgressText());
        break;
    }
    pipelineProgressMonitor->progressEvent(alg, msg, percent);
}


