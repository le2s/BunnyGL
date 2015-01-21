CONFIG += gcf_app
DESTDIR = $$PWD/../../../../bin/
QT += declarative phonon

SOURCES += \
    Main.cpp \
    MediaPlayerWindow.cpp \
    MediaPlayer.cpp \
    AudioLibraryModel.cpp \
    MediaPlayerComponent.cpp \
    VideoLibraryModel.cpp

HEADERS += \
    MediaPlayerWindow.h \
    MediaPlayer.h \
    AudioLibraryModel.h \
    MediaPlayerComponent.h \
    VideoLibraryModel.h \
    MediaPlayer_p.h

FORMS += \
    MediaPlayerWindow.ui \
    PlaybackControls.ui

OTHER_FILES += \
    QML/AudioLibrary.qml \
    MediaPlayerComponent.xml \
    Application.xml \
    QML/LibraryDelegate.qml \
    QML/VideoLibrary.qml

RESOURCES += \
    MediaPlayerDesktop.qrc
