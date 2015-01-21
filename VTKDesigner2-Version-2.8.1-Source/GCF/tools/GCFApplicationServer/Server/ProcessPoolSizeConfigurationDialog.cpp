#include "ProcessPoolSizeConfigurationDialog.h"
#include "RequestHandlerPool.h"
#include "ui_ProcessPoolSizeConfigurationDialog.h"

struct ProcessPoolSizeConfigurationDialogData
{
    Ui::ProcessPoolSizeConfigurationDialog ui;

    RequestHandlerPool* requestHandlerPool;
};

ProcessPoolSizeConfigurationDialog::ProcessPoolSizeConfigurationDialog(RequestHandlerPool* requestHandlerPool, QWidget* parent)
    :QDialog(parent)
{
    d = new ProcessPoolSizeConfigurationDialogData;

    d->requestHandlerPool = requestHandlerPool;
    d->ui.setupUi(this);

    this->setFixedSize(400, 100);

    d->ui.sbNumberOfProcesses->setValue(d->requestHandlerPool->poolSize());
    connect(d->ui.pbOk, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
    connect(d->ui.pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

ProcessPoolSizeConfigurationDialog::~ProcessPoolSizeConfigurationDialog()
{
    delete d;
}

void ProcessPoolSizeConfigurationDialog::okButtonClicked()
{
    if(d->ui.sbNumberOfProcesses->value() < 1)
        return;

    if(d->ui.sbNumberOfProcesses->value() == d->requestHandlerPool->poolSize())
        QDialog::accept();

    d->requestHandlerPool->setPoolSize(d->ui.sbNumberOfProcesses->value());
    QDialog::accept();
}
