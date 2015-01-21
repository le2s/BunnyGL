#ifndef AUDIOLIBRARYMODEL_H
#define AUDIOLIBRARYMODEL_H

#include <QStandardItemModel>

class AudioLibraryModel : public QStandardItemModel
{
    Q_OBJECT

public:
    AudioLibraryModel(QObject *parent = 0);
    ~AudioLibraryModel();

    enum Role
    {
        FileName = Qt::DisplayRole,
        FileGroup = Qt::UserRole+1,
        AbsoluteFileName = Qt::UserRole+2,
        FileType = Qt::UserRole+3
    };

private:
    void init();
    void loadMusic(const QString& musicDirPath);
};

#endif // AUDIOLIBRARYMODEL_H
