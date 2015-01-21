#include "ServerLoadInfoDialog.h"
#include "RequestHandlerPool.h"
#include "ui_ServerLoadInfoDialog.h"
#include "ApplicationServerComponent.h"

#include <QMap>
#include <QDebug>
#include <QVariant>
#include <QProcess>

qlonglong procPID(QProcess* proc);

struct ServerLoadInfoDialogData
{
    Ui::ServerLoadInfoDialog ui;

    const RequestHandlerPool* requestHandlerPool;
};

ServerLoadInfoDialog::ServerLoadInfoDialog(const RequestHandlerPool* requestHandlerPool, QWidget* parent)
    :QDialog(parent)
{
    d = new ServerLoadInfoDialogData;

    d->requestHandlerPool = requestHandlerPool;
    d->ui.setupUi(this);

    initialize();
}

ServerLoadInfoDialog::~ServerLoadInfoDialog()
{
    delete d;
}

void ServerLoadInfoDialog::initialize()
{
    if(!d->requestHandlerPool)
        return;

    const QMap< QProcess*, QList<RequestorSocket*> > processSocketMap = d->requestHandlerPool->processSocketMap();

    if(processSocketMap.isEmpty())
        return;

    int processCounter = 1;
    QMap<QProcess*, QList<RequestorSocket*> >::const_iterator it = processSocketMap.begin();
    QMap<QProcess*, QList<RequestorSocket*> >::const_iterator end = processSocketMap.end();
    while(it != end)
    {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(QString("Process - %1").arg(processCounter));

        QVariant procPointerVariant = qVariantFromValue<void*>((void*)(it.key()));
        item->setData(Qt::UserRole, procPointerVariant);

        d->ui.processListWidget->addItem(item);

        ++processCounter;
        ++it;
    }

    connect(d->ui.processListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(loadProcessInfo(QListWidgetItem*,QListWidgetItem*)));
}

void ServerLoadInfoDialog::loadProcessInfo(QListWidgetItem* current,QListWidgetItem* previous)
{
    Q_UNUSED(previous);

    QVariant ptrVariant = current->data(Qt::UserRole);
    QProcess* process = reinterpret_cast<QProcess*>(ptrVariant.value<void*>());

    if(!process)
        return;

    d->ui.processIDLineEdit->setText(QString::number(procPID(process)));
    d->ui.currentRequestCountLineEdit->setText(
            QString::number(d->requestHandlerPool->processSocketMap().value(process).count()));

    d->ui.associatedGUIDsListWidget->clear();
    Q_FOREACH(QString string, process->property("GUIDList").toStringList())
        d->ui.associatedGUIDsListWidget->addItem(string);
}
