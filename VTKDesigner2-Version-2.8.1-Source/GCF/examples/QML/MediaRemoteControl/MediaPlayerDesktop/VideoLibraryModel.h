#ifndef VIDEOLIBRARYMODEL_H
#define VIDEOLIBRARYMODEL_H

#include <QStandardItemModel>

class VideoLibraryModel : public QStandardItemModel
{
    Q_OBJECT

public:
    VideoLibraryModel(QObject *parent = 0);
    ~VideoLibraryModel();
    
    enum Role
    {
        FileName = Qt::DisplayRole,
        FileGroup = Qt::UserRole+1,
        AbsoluteFileName = Qt::UserRole+2,
        FileType = Qt::UserRole+3
    };

private:
    void init();
    void loadVideos(const QString& videoDirPath);
};

#endif // VIDEOLIBRARYMODEL_H
