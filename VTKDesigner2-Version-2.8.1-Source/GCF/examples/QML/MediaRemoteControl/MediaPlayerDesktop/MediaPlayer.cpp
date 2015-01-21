#include "MediaPlayer.h"
#include "MediaPlayer_p.h"

#include <QFont>
#include <QPainter>
#include <QStyle>
#include <QFile>
#include <QTime>
#include <QTimer>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>

#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <phonon/VideoWidget>
#include <phonon/VolumeSlider>

#include <QStackedWidget>
#include <QLabel>
#include <QVBoxLayout>

struct MediaPlayerData
{
    QStackedWidget* stackedWidget;
    QWidget* audioWidget;
    Phonon::VideoWidget* videoWidget;
    Phonon::AudioOutput* audioOutput;
    PlaybackControls* playbackControls;
    QString mediaFile;
    Phonon::MediaObject* mediaObject;
};

MediaPlayer::MediaPlayer(QWidget *parent) :
    QWidget(parent)
{
    d = new MediaPlayerData;

    this->setLayout(new QVBoxLayout);
    this->layout()->setMargin(2);
    this->layout()->setSpacing(2);

    d->audioOutput = new Phonon::AudioOutput(this);

    d->stackedWidget = new QStackedWidget(this);
    this->layout()->addWidget(d->stackedWidget);

    d->playbackControls = new PlaybackControls(this);
    this->layout()->addWidget(d->playbackControls);

    d->audioWidget = new QWidget;
    d->audioWidget->setStyleSheet("background: rgba(128,128,128,255)");
    QVBoxLayout* audioWidgetLayout = new QVBoxLayout(d->audioWidget);
    audioWidgetLayout->setAlignment(Qt::AlignCenter);
    QLabel* audioWidgetLabel = new QLabel;
    audioWidgetLabel->setPixmap(QPixmap(":/Icons/audio_icon.png"));
    audioWidgetLabel->setFixedSize(128, 128);
    audioWidgetLayout->addWidget(audioWidgetLabel);
    d->stackedWidget->addWidget(d->audioWidget);

    d->videoWidget = new Phonon::VideoWidget;
    d->stackedWidget->addWidget(d->videoWidget);

    d->stackedWidget->setCurrentIndex(0);
    d->mediaObject = 0;
    this->createMediaObject();
}

MediaPlayer::~MediaPlayer()
{
    delete d;
}

void MediaPlayer::setMedia(const QString& mediaFile)
{
    this->stop(); // wait for 500 ms after
    QEventLoop eventLoop;
    QTime time = QTime::currentTime();
    while(time.msecsTo(QTime::currentTime()) < 500)
        eventLoop.processEvents();
    this->createMediaObject();

    d->mediaFile = mediaFile;
    if(d->mediaFile.isEmpty() || !QFile::exists(d->mediaFile))
    {
        d->mediaFile.clear();
        emit mediaChanged();
        return;
    }

    d->mediaObject->enqueue(Phonon::MediaSource(d->mediaFile));
    d->mediaObject->play();
    emit mediaChanged();
}

QString MediaPlayer::media() const
{
    return d->mediaFile;
}

bool MediaPlayer::isPlaying() const
{
    return d->playbackControls->isPlaying();
}

bool MediaPlayer::isPlayingAudio() const
{
    if(!this->isPlaying())
        return false;

    return d->stackedWidget->currentIndex() == 0;
}

bool MediaPlayer::isPlayingVideo() const
{
    if(!this->isPlaying())
        return false;

    return d->stackedWidget->currentIndex() == 1;
}

bool MediaPlayer::isPlayingFullscreen() const
{
    return (d->playbackControls->isVisible() == false);
}

void MediaPlayer::play()
{
    d->playbackControls->play();
}

void MediaPlayer::pause()
{
    d->playbackControls->pause();
}

void MediaPlayer::rewind()
{
    d->playbackControls->rewind();
}

void MediaPlayer::forward()
{
    d->playbackControls->forward();
}

void MediaPlayer::next()
{
    d->playbackControls->next();
}

void MediaPlayer::previous()
{
    d->playbackControls->previous();
}

void MediaPlayer::stop()
{
    d->playbackControls->stop();
}

void MediaPlayer::volumeUp()
{
    d->playbackControls->volumeUp();
}

void MediaPlayer::volumeDown()
{
    d->playbackControls->volumeDown();
}

void MediaPlayer::mute()
{
    d->playbackControls->mute();
}

void MediaPlayer::unmute()
{
    d->playbackControls->unmute();
}

void MediaPlayer::fullscreen()
{
    if(!this->isPlayingVideo() || this->isPlayingFullscreen())
        return;

    d->playbackControls->hide();
    qApp->installEventFilter(this);
    this->layout()->setMargin(0);
    emit fullScreenRequest();
}

void MediaPlayer::normalScreen()
{
    if(!this->isPlayingVideo() || !this->isPlayingFullscreen())
        return;

    d->playbackControls->show();
    qApp->removeEventFilter(this);
    this->layout()->setMargin(2);
    emit normalScreenRequest();
}

void MediaPlayer::setVideoVisible(bool val)
{
    if(val)
        d->stackedWidget->setCurrentIndex(1);
    else
        d->stackedWidget->setCurrentIndex(0);
}

void MediaPlayer::notifyError()
{
    Phonon::ErrorType errType = d->mediaObject->errorType();
    switch(errType)
    {
    case Phonon::NoError:
        return;
    case Phonon::NormalError:
        QMessageBox::information(this->window(), "Warning", d->mediaObject->errorString());
        break;
    case Phonon::FatalError:
        QMessageBox::critical(this->window(), "Fatal Error", d->mediaObject->errorString());
        QTimer::singleShot(200, this, SLOT(createMediaObject()));
        d->mediaFile.clear();
        break;
    }
}

bool MediaPlayer::eventFilter(QObject *, QEvent *e)
{
    if(this->isPlayingFullscreen())
    {
        if(e->type() == QEvent::KeyPress)
        {
            QKeyEvent* ke = (QKeyEvent*)e;
            switch(ke->key())
            {
            case Qt::Key_Escape:
                this->normalScreen();
                break;
            default:
                break;
            }
        }
    }

    return false;
}

void MediaPlayer::createMediaObject()
{
    if(d->mediaObject)
    {
        d->playbackControls->setMediaObject(0);
        delete d->mediaObject;
    }

    this->window()->setWindowTitle(qApp->applicationName());

    d->mediaObject = new Phonon::MediaObject(this);
    Phonon::createPath(d->mediaObject, d->audioOutput);
    Phonon::createPath(d->mediaObject, d->videoWidget);
    d->playbackControls->setMediaObject(d->mediaObject);
}

///////////////////////////////////////////////////////////////////////////////

#include "ui_PlaybackControls.h"

struct PlaybackControlsData
{
    Ui::PlaybackControls ui;

    bool isPlaying;
    QIcon playIcon;
    QIcon pauseIcon;
    MediaPlayer* mediaPlayer;
    Phonon::MediaObject* mediaObject;
};

PlaybackControls::PlaybackControls(MediaPlayer* parent)
    : QWidget(parent)
{
    d = new PlaybackControlsData;
    d->ui.setupUi(this);
    d->mediaPlayer = parent;

    d->playIcon = this->style()->standardIcon(QStyle::SP_MediaPlay);
    d->pauseIcon = this->style()->standardIcon(QStyle::SP_MediaPause);
    d->ui.cmdPlayPause->setIcon(d->playIcon);
    d->ui.cmdStop->setIcon(this->style()->standardIcon(QStyle::SP_MediaStop));
    d->ui.cmdPrevious->setIcon(this->style()->standardIcon(QStyle::SP_MediaSkipBackward));
    d->ui.cmdNext->setIcon(this->style()->standardIcon(QStyle::SP_MediaSkipForward));
    d->ui.cmdRewind->setIcon(this->style()->standardIcon(QStyle::SP_MediaSeekBackward));
    d->ui.cmdForward->setIcon(this->style()->standardIcon(QStyle::SP_MediaSeekForward));
    d->ui.cmdFullscreen->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    d->ui.volumeSlider->setAudioOutput(parent->d->audioOutput);
    d->ui.volumeSlider->setMuteVisible(true);
    d->mediaObject = 0;
    this->setEnabled(false);

    connect(d->ui.cmdForward, SIGNAL(clicked()), parent, SLOT(forward()));
    connect(d->ui.cmdRewind, SIGNAL(clicked()), parent, SLOT(rewind()));
    connect(d->ui.cmdNext, SIGNAL(clicked()), parent, SLOT(next()));
    connect(d->ui.cmdPrevious, SIGNAL(clicked()), parent, SLOT(previous()));
    connect(d->ui.cmdStop, SIGNAL(clicked()), parent, SLOT(stop()));
}

PlaybackControls::~PlaybackControls()
{
    delete d;
}

void PlaybackControls::setMediaObject(Phonon::MediaObject* object)
{
    if(d->mediaObject == object)
        return;

    if(d->mediaObject)
        disconnect(d->mediaObject, 0, this, 0);

    d->mediaObject = object;
    d->ui.seekSlider->setMediaObject(d->mediaObject);
    if(!d->mediaObject)
    {
        this->setEnabled(false);
        return;
    }

    connect(d->mediaObject, SIGNAL(aboutToFinish()),
            this, SLOT(onMediaObject_aboutToFinish()));
    connect(d->mediaObject, SIGNAL(bufferStatus(int)),
            this, SLOT(onMediaObject_bufferStatus(int)));
    connect(d->mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
            this, SLOT(onMediaObject_currentSourceChanged(Phonon::MediaSource)));
    connect(d->mediaObject, SIGNAL(finished()),
            this, SLOT(onMediaObject_finished()));
    connect(d->mediaObject, SIGNAL(hasVideoChanged(bool)),
            this, SLOT(onMediaObject_hasVideoChanged(bool)));
    connect(d->mediaObject, SIGNAL(metaDataChanged()),
            this, SLOT(onMediaObject_metaDataChanged()));
    connect(d->mediaObject, SIGNAL(prefinishMarkReached(qint32)),
            this, SLOT(onMediaObject_prefinishMarkReached(qint32)));
    connect(d->mediaObject, SIGNAL(seekableChanged(bool)),
            this, SLOT(onMediaObject_seekableChanged(bool)));
    connect(d->mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(onMediaObject_stateChanged(Phonon::State, Phonon::State)));
    connect(d->mediaObject, SIGNAL(tick(qint64)),
            this, SLOT(onMediaObject_tick(qint64)));
    connect(d->mediaObject, SIGNAL(totalTimeChanged(qint64)),
            this, SLOT(onMediaObject_totalTimeChanged(qint64)));

    this->setEnabled(true);
}

Phonon::MediaObject* PlaybackControls::mediaObject() const
{
    return d->mediaObject;
}

bool PlaybackControls::isPlaying() const
{
    if(!d->mediaObject)
        return false;

    return d->mediaObject->state() == Phonon::PlayingState;
}

void PlaybackControls::play()
{
    if(d->mediaObject)
        d->mediaObject->play();
}

void PlaybackControls::pause()
{
    if(d->mediaObject)
        d->mediaObject->pause();
}

void PlaybackControls::rewind()
{
    if(d->mediaObject)
    {
        qint64 time = d->mediaObject->currentTime();
        time -= 500;
        if(time < 0)
            time = 0;
        d->mediaObject->seek(time);
    }
}

void PlaybackControls::forward()
{
    if(d->mediaObject)
    {
        qint64 time = d->mediaObject->currentTime();
        time += 500;
        if(time >= d->mediaObject->totalTime())
            time = d->mediaObject->totalTime();
        d->mediaObject->seek(time);
    }
}

void PlaybackControls::next()
{
    if(d->mediaObject)
    {
        qint64 time = d->mediaObject->currentTime();
        time += 2*60*1000; // 2 minutes
        if(time >= d->mediaObject->totalTime())
            time = d->mediaObject->totalTime();
        d->mediaObject->seek(time);
    }
}

void PlaybackControls::previous()
{
    if(d->mediaObject)
    {
        qint64 time = d->mediaObject->currentTime();
        time -= 2*60*1000; // 2 minutes
        if(time < 0)
            time = 0;
        d->mediaObject->seek(time);
    }
}

void PlaybackControls::stop()
{
    if(d->mediaObject)
        d->mediaObject->stop();
}

void PlaybackControls::volumeUp()
{
    qreal currentVolume = d->ui.volumeSlider->audioOutput()->volume();
    qreal maxVolume = d->ui.volumeSlider->maximumVolume();
    qreal volume = currentVolume + 0.1*maxVolume;
    d->ui.volumeSlider->audioOutput()->setVolume(volume);
}

void PlaybackControls::volumeDown()
{
    qreal currentVolume = d->ui.volumeSlider->audioOutput()->volume();
    qreal maxVolume = d->ui.volumeSlider->maximumVolume();
    qreal volume = currentVolume - 0.1*maxVolume;
    d->ui.volumeSlider->audioOutput()->setVolume(volume);
}

void PlaybackControls::mute()
{
    d->ui.volumeSlider->audioOutput()->setMuted(true);
}

void PlaybackControls::unmute()
{
    d->ui.volumeSlider->audioOutput()->setMuted(false);
}

void PlaybackControls::on_cmdPlayPause_clicked()
{
    if(this->isPlaying())
        this->pause();
    else
        this->play();
}

void PlaybackControls::on_cmdFullscreen_clicked()
{
    d->mediaPlayer->fullscreen();
}

void PlaybackControls::onMediaObject_aboutToFinish()
{

}

void PlaybackControls::onMediaObject_bufferStatus(int percentFilled)
{
    // Until we deal with online videos, lets not bother about this
    Q_UNUSED(percentFilled);
}

void PlaybackControls::onMediaObject_currentSourceChanged(const Phonon::MediaSource & newSource)
{
    QString title = qApp->applicationName();
    QString mediaFileName = newSource.fileName();
    if(!mediaFileName.isEmpty())
        title = QString("%1 - %2").arg(mediaFileName).arg(title);
    this->window()->setWindowTitle(title);
}

void PlaybackControls::onMediaObject_finished()
{

}

void PlaybackControls::onMediaObject_hasVideoChanged(bool hasVideo)
{
    d->mediaPlayer->setVideoVisible(hasVideo);
}

void PlaybackControls::onMediaObject_metaDataChanged()
{

}

void PlaybackControls::onMediaObject_prefinishMarkReached(qint32 msecToEnd)
{
    Q_UNUSED(msecToEnd);
}

void PlaybackControls::onMediaObject_seekableChanged(bool isSeekable)
{
    d->ui.cmdNext->setEnabled(isSeekable);
    d->ui.cmdPrevious->setEnabled(isSeekable);
    d->ui.cmdRewind->setEnabled(isSeekable);
    d->ui.cmdForward->setEnabled(isSeekable);
}

void PlaybackControls::onMediaObject_stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    switch(newstate)
    {
    case Phonon::LoadingState:
    case Phonon::BufferingState:
        qApp->setOverrideCursor(Qt::WaitCursor);
        break;
    case Phonon::PlayingState:
        qApp->restoreOverrideCursor();
        d->ui.cmdPlayPause->setIcon(d->pauseIcon);
        break;
    case Phonon::PausedState:
        d->ui.cmdPlayPause->setIcon(d->playIcon);
        break;
    case Phonon::StoppedState:
        d->ui.cmdPlayPause->setIcon(d->playIcon);
        break;
    case Phonon::ErrorState:
        d->mediaPlayer->notifyError();
        break;
    default:
        break;
    }

    Q_UNUSED(oldstate);
}

void PlaybackControls::onMediaObject_tick(qint64 time)
{
    Q_UNUSED(time);
}

void PlaybackControls::onMediaObject_totalTimeChanged(qint64 newTotalTime)
{
    Q_UNUSED(newTotalTime);
}

