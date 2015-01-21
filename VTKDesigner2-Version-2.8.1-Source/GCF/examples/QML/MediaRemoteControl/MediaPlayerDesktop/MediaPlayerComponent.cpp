#include "MediaPlayerComponent.h"
#include "MediaPlayerWindow.h"
#include "AudioLibraryModel.h"
#include "VideoLibraryModel.h"
#include "MediaPlayer.h"

#include <GCF/Application>

GCF_DEFINE_COMPONENT(MediaPlayerComponent)

struct MediaPlayerComponentData
{
    MediaPlayerWindow* mediaPlayerWindow;
    AudioLibraryModel audioLibraryModel;
    VideoLibraryModel videoLibraryModel;
};

MediaPlayerComponent& MediaPlayerComponent::instance()
{
    return *(GCF_CREATE_COMPONENT(MediaPlayerComponent));
}

MediaPlayerComponent::MediaPlayerComponent()
{
    d = new MediaPlayerComponentData;
    d->mediaPlayerWindow = new MediaPlayerWindow;
    gApp->setComponentHostWidget(d->mediaPlayerWindow);
}

MediaPlayerComponent::~MediaPlayerComponent()
{
    delete d;
}

void MediaPlayerComponent::initializeComponent()
{
    this->setComponentName("MediaPlayerComponent");
    
    // TODO: Either set the GUIXML file from a resource file or 
    // allow GCF to load a GUIXML file from the GUIXML directory.
    this->setGuiXmlFileName(":/MediaPlayerComponent.xml");
}

void MediaPlayerComponent::finalizeComponent()
{
}

void MediaPlayerComponent::creationAnnounced()
{
    d->mediaPlayerWindow->showMaximized();
}

QObject* MediaPlayerComponent::fetchObject(const QString &completeName) const
{
    if(completeName == "MediaPlayerComponent.Core")
        return const_cast<MediaPlayerComponent*>(this);

    if(completeName == "MediaPlayerComponent.audioLibraryModel")
        return &d->audioLibraryModel;

    if(completeName == "MediaPlayerComponent.videoLibraryModel")
        return &d->videoLibraryModel;

    return 0;
}

QWidget* MediaPlayerComponent::fetchWidget(const QString &completeName) const
{
    if(completeName == "MediaPlayerComponent.mediaPlayer")
        return d->mediaPlayerWindow->mediaPlayer();

    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(MediaPlayerComponent)

