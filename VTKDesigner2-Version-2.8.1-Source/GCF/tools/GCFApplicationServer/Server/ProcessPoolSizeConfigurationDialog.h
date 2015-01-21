#ifndef PROCESS_POOL_SIZE_CONFIGURATION_DIALOG_H
#define PROCESS_POOL_SIZE_CONFIGURATION_DIALOG_H

#include <QDialog>

class RequestHandlerPool;

struct ProcessPoolSizeConfigurationDialogData;
class ProcessPoolSizeConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    ProcessPoolSizeConfigurationDialog(RequestHandlerPool* requestHandlerPool, QWidget* parent = 0);
    ~ProcessPoolSizeConfigurationDialog();

private slots:
    void okButtonClicked();

private:
    ProcessPoolSizeConfigurationDialogData* d;
};

#endif // PROCESS_POOL_SIZE_CONFIGURATION_DIALOG_H
