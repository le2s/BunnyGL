#ifndef SERVER_LOAD_INFO_DIALOG_H
#define SERVER_LOAD_INFO_DIALOG_H

#include <QDialog>
#include <QProcess>

class QListWidgetItem;
class RequestHandlerPool;

struct ServerLoadInfoDialogData;
class ServerLoadInfoDialog : public QDialog
{
    Q_OBJECT

public:
    ServerLoadInfoDialog(const RequestHandlerPool* requestHandlerPool, QWidget* parent = 0);
    ~ServerLoadInfoDialog();

private:
    void initialize();

private slots:
    void loadProcessInfo(QListWidgetItem* current,QListWidgetItem* previous);

private:
    ServerLoadInfoDialogData* d;
};

#endif // SERVER_LOAD_INFO_DIALOG_H
