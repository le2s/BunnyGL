#ifndef MEDIAPLAYERWINDOW_H
#define MEDIAPLAYERWINDOW_H

#include <QMainWindow>

class MediaPlayer;

struct MediaPlayerWindowData;
class MediaPlayerWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MediaPlayerWindow(QWidget *parent = 0);
    ~MediaPlayerWindow();

    MediaPlayer* mediaPlayer() const;

protected:
    void showEvent(QShowEvent *);

private slots:
    void onFullScreenRequest();
    void onNormalScreenRequest();
    
private:
    MediaPlayerWindowData* d;
};

#endif // MEDIAPLAYERWINDOW_H
