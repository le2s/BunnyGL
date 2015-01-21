#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>

class PlaybackControls;

struct MediaPlayerData;
class MediaPlayer : public QWidget
{
    Q_OBJECT

public:
    MediaPlayer(QWidget *parent = 0);
    ~MediaPlayer();

    Q_PROPERTY(QString media READ media WRITE setMedia NOTIFY mediaChanged)
    void setMedia(const QString& mediaFile);
    QString media() const;

    bool isPlaying() const;
    bool isPlayingAudio() const;
    bool isPlayingVideo() const;
    bool isPlayingFullscreen() const;

signals:
    void mediaChanged();
    void fullScreenRequest();
    void normalScreenRequest();

public slots:
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
    void fullscreen();
    void normalScreen();

private:
    void setVideoVisible(bool val);
    void notifyError();
    bool eventFilter(QObject *, QEvent *);

private slots:
    void createMediaObject();
    
private:
    friend class PlaybackControls;
    MediaPlayerData* d;
};

#endif // MEDIAPLAYER_H
