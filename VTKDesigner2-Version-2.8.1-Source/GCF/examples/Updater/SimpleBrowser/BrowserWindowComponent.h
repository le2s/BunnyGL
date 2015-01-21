#ifndef BROWSER_WINDOW_COMPONENT_H
#define BROWSER_WINDOW_COMPONENT_H

#include <GCF/AbstractComponent>

struct BrowserWindowComponentData;
class BrowserWindowComponent : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(BrowserWindowComponent)

    public:
        static BrowserWindowComponent& instance();
    ~BrowserWindowComponent();

protected:
    BrowserWindowComponent();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void initializeComponent();
    void finalizeComponent();

private:
    BrowserWindowComponentData* d;
};

#endif
