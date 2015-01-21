#include "ErrorMessageDialog.h"
#include "ui_ErrorMessageDialog.h"
#include "ErrorDump.h"
#include "ErrorMessage.h"

#include <QPushButton>

GCF::ErrorMessageDialog::ErrorMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorMessageDialog)
{
    ui->setupUi(this);
    ui->lstErrorMessages->setModel( &GCF::ErrorDump::instance() );
    ui->lstErrorMessages->hide();
    if(GCF::ErrorDump::instance().supportEMail().isEmpty())
        ui->cmdSubmitToSupport->hide();

    connect(&GCF::ErrorDump::instance(), SIGNAL(modelReset()), this, SLOT(onErrorDumpReset()));
    this->setFixedHeight(200);

    this->setWindowTitle( QString("Error - %1").arg(qApp->applicationName()) );

    qApp->setOverrideCursor(Qt::ArrowCursor);
}

GCF::ErrorMessageDialog::~ErrorMessageDialog()
{
    qApp->restoreOverrideCursor();

    delete ui;
}

void GCF::ErrorMessageDialog::on_cmdShowHideDetails_clicked()
{
    if(ui->lstErrorMessages->isVisible())
    {
        ui->lstErrorMessages->hide();
        ui->cmdShowHideDetails->setText( tr("Show &Details") );
        this->setFixedHeight(200);
    }
    else
    {
        ui->lstErrorMessages->show();
        ui->cmdShowHideDetails->setText( tr("Hide &Details") );
        this->setFixedHeight(450);
    }
}

void GCF::ErrorMessageDialog::on_cmdCopyToClipboard_clicked()
{
    GCF::ErrorDump::instance().copyToClipboard();
}

void GCF::ErrorMessageDialog::on_cmdSubmitToSupport_clicked()
{
    GCF::ErrorDump::instance().submitToSupport();
}

void GCF::ErrorMessageDialog::onErrorDumpReset()
{
    QList<GCF::ErrorMessage*> messages = GCF::ErrorDump::instance().errorMessages();
    QString msg;
    if(messages.count())
    {
        msg = messages.first()->message();
        if(!messages.first()->details().isEmpty())
            msg += "\n\n" + messages.first()->message();
    }

    if(msg.trimmed().isEmpty())
        msg = tr("An error was encountered in your application");

    ui->lblErrorMessage->setText(msg);

    ui->lstErrorMessages->expandAll();
    ui->lstErrorMessages->setColumnHidden(2, true);
    ui->lstErrorMessages->resizeColumnToContents(5);
    ui->lstErrorMessages->resizeColumnToContents(3);
    ui->lstErrorMessages->resizeColumnToContents(1);
    ui->lstErrorMessages->resizeColumnToContents(0);
    // ui->lstErrorMessages->setRootIsDecorated(false);
}

void GCF::ErrorMessageDialog::showEvent(QShowEvent *)
{
    this->onErrorDumpReset();
}

