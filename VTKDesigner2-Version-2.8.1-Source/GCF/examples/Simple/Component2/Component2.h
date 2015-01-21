#ifndef COMPONENT2_H
#define COMPONENT2_H

#include <GCF/AbstractComponent>

struct Component2Data;
class Component2 : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(Component2)

    public:
        static Component2& instance();
    ~Component2();

protected:
    Component2();
    void initializeComponent();
    void finalizeComponent();
    QWidget* fetchWidget(const QString &completeName) const;

private:
    Component2Data* d;
};

#endif // COMPONENT2_H
