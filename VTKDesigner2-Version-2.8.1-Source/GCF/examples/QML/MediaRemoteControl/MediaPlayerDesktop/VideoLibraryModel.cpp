#include "VideoLibraryModel.h"

#include <QFileSystemWatcher>
#include <QDesktopServices>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

VideoLibraryModel::VideoLibraryModel(QObject *parent) :
    QStandardItemModel(parent)
{
    QHash<int,QByteArray> roleNames;
    roleNames[FileName] = "fileName";
    roleNames[FileType] = "fileType";
    roleNames[FileGroup] = "fileGroup";
    roleNames[AbsoluteFileName] = "filePath";
    this->setRoleNames(roleNames);

    this->init();
}

VideoLibraryModel::~VideoLibraryModel()
{

}

void VideoLibraryModel::init()
{
    QString videosDir = QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
    this->loadVideos(videosDir);

    // On Windows 7 / Vista - there is also a public video folder
    videosDir = "C:/Users/Public/Videos"; // ok, we are hardcoding the public video folder..
    this->loadVideos(videosDir);
}

void VideoLibraryModel::loadVideos(const QString& videoDirPath)
{
    static QStringList allowedSuffixes = QStringList() << "mov" << "avi"
        << "divx" << "mp4" << "dat" << "mpg" << "mpeg";

    QDir dir(videoDirPath);
    QFileInfoList fiList = dir.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot, QDir::Name|QDir::DirsLast);
    Q_FOREACH(QFileInfo fi, fiList)
    {
        if(fi.isFile())
        {
            if(!allowedSuffixes.contains(fi.suffix().toLower()))
                continue;

            QString fileName = fi.absoluteFilePath().split("/").last();
            while(1)
            {
                QChar ch = fileName.at(fileName.length()-1);
                fileName.remove(fileName.length()-1, 1);
                if(ch == '.')
                    break;
            }

            QStandardItem* item = new QStandardItem;
            item->setData(fileName, FileName);
            item->setData(fi.suffix().toUpper(), FileType);
            item->setData(fi.absoluteFilePath(), AbsoluteFileName);
            item->setData(fi.dir().dirName(), FileGroup);
            this->appendRow(item);
        }
        else
            this->loadVideos(fi.absoluteFilePath());
    }
}
