#ifndef QMLCOMPONENT_H
#define QMLCOMPONENT_H

#include <GCF/AbstractComponent>

namespace GCF
{

class Application;

struct QMLComponentData;
class QMLComponent : public GCF::AbstractComponent
{
    Q_OBJECT

public:
    ~QMLComponent();

    QString qmlComponentPath() const;

protected:
    QMLComponent(const QString& componentName,
                 const QString& guixmlFileName,
                 QObject *parent = 0);
    void initializeComponent();
    void finalizeComponent();
    QWidget* fetchWidget(const QString &completeName) const;
    QObject* fetchObject(const QString &completeName) const;

private:
    friend class Application;
    QMLComponentData* d;
};

}

#endif // QMLCOMPONENT_H
