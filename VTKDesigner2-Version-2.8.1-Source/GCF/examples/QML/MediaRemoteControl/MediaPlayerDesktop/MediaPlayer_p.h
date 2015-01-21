#ifndef MEDIAPLAYER_P_H
#define MEDIAPLAYER_P_H

#include <QWidget>
#include <phonon/MediaObject>

class MediaPlayer;

struct PlaybackControlsData;
class PlaybackControls : public QWidget
{
    Q_OBJECT

public:
    PlaybackControls(MediaPlayer* parent=0);
    ~PlaybackControls();

    void setMediaObject(Phonon::MediaObject* object);
    Phonon::MediaObject* mediaObject() const;

    bool isPlaying() const;

    void play();
    void pause();
    void rewind();
    void forward();
    void next();
    void previous();
    void stop();
    void volumeUp();
    void volumeDown();
    void mute();
    void unmute();

private slots:
    void on_cmdPlayPause_clicked();
    void on_cmdFullscreen_clicked();

    void onMediaObject_aboutToFinish();
    void onMediaObject_bufferStatus(int percentFilled);
    void onMediaObject_currentSourceChanged(const Phonon::MediaSource & newSource);
    void onMediaObject_finished();
    void onMediaObject_hasVideoChanged(bool hasVideo);
    void onMediaObject_metaDataChanged();
    void onMediaObject_prefinishMarkReached(qint32 msecToEnd);
    void onMediaObject_seekableChanged(bool isSeekable);
    void onMediaObject_stateChanged(Phonon::State newstate, Phonon::State oldstate);
    void onMediaObject_tick(qint64 time);
    void onMediaObject_totalTimeChanged(qint64 newTotalTime);

private:
    friend class MediaPlayer;
    PlaybackControlsData* d;
};

#endif // MEDIAPLAYER_P_H
