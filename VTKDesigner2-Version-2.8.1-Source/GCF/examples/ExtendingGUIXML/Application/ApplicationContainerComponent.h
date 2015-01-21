#ifndef APPLICATIONCONTAINERCOMPONENT_H
#define APPLICATIONCONTAINERCOMPONENT_H

#include <GCF/AbstractComponent>

class QTabWidget;

struct ApplicationContainerComponentData;
class ApplicationContainerComponent : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(ApplicationContainerComponent)

public:
    static ApplicationContainerComponent& instance();
    ~ApplicationContainerComponent();

    QTabWidget* menuWidget() const;
    QTabWidget* workspace() const;

protected:
    ApplicationContainerComponent();
    void initializeComponent();
    void finalizeComponent();
    QWidget* fetchWidget(const QString &completeName) const;
    QObject* fetchObject(const QString &completeName) const;
    void addChildWidget(QWidget *parent, QWidget *child, const QString &hint);
    void removeChildWidget(QWidget *parent, QWidget *child, const QString &hint);

private:
    ApplicationContainerComponentData* d;
};

#endif // APPLICATIONCONTAINERCOMPONENT_H
