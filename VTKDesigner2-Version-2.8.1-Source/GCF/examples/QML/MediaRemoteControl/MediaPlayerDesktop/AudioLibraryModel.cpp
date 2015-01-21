#include "AudioLibraryModel.h"

#include <QFileSystemWatcher>
#include <QDesktopServices>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

AudioLibraryModel::AudioLibraryModel(QObject *parent) :
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

AudioLibraryModel::~AudioLibraryModel()
{

}

void AudioLibraryModel::init()
{
    QString musicDir = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
    this->loadMusic(musicDir);

    // On Windows 7 / Vista - there is also a public music folder
    musicDir = "C:/Users/Public/Music"; // ok, we are hardcoding the public music folder..
    this->loadMusic(musicDir);
}

void AudioLibraryModel::loadMusic(const QString& musicDirPath)
{
    static QStringList allowedSuffixes = QStringList() << "mp3" << "wav" << "wma";

    QDir dir(musicDirPath);
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
            this->loadMusic(fi.absoluteFilePath());
    }
}

