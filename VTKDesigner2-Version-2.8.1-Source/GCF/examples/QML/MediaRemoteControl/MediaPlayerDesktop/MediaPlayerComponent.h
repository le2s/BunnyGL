#ifndef MEDIAPLAYERCOMPONENT_H
#define MEDIAPLAYERCOMPONENT_H

#include <GCF/AbstractComponent>

struct MediaPlayerComponentData;
class MediaPlayerComponent : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(MediaPlayerComponent)
    
public:
    static MediaPlayerComponent& instance();
    ~MediaPlayerComponent();
    
protected:
    MediaPlayerComponent();
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();
    QObject* fetchObject(const QString &completeName) const;
    QWidget* fetchWidget(const QString &completeName) const;
    
private:
    MediaPlayerComponentData* d;
};

#endif // MEDIAPLAYERCOMPONENT_H

