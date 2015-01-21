#ifndef ERRORMESSAGEDIALOG_H
#define ERRORMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class ErrorMessageDialog;
}

class QAbstractButton;

namespace GCF
{

class ErrorMessageDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ErrorMessageDialog(QWidget *parent = 0);
    ~ErrorMessageDialog();

protected slots:
    void on_cmdShowHideDetails_clicked();
    void on_cmdCopyToClipboard_clicked();
    void on_cmdSubmitToSupport_clicked();
    void onErrorDumpReset();

protected:
    void showEvent(QShowEvent *);
    
private:
    Ui::ErrorMessageDialog *ui;
};

}

#endif // ERRORMESSAGEDIALOG_H
