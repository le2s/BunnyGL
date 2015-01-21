#ifndef COMPONENT1_H
#define COMPONENT1_H

#include <GCF/AbstractComponent>

struct Component1Data;
class Component1 : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(Component1)

    public:
        static Component1& instance();
    ~Component1();

protected:
    Component1();
    void initializeComponent();
    void finalizeComponent();
    QWidget* fetchWidget(const QString &completeName) const;

private:
    Component1Data* d;
};

#endif // COMPONENT1_H
