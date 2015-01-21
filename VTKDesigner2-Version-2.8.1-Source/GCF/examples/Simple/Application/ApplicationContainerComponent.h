#ifndef APPLICATIONCONTAINERCOMPONENT_H
#define APPLICATIONCONTAINERCOMPONENT_H

#include <GCF/AbstractComponent>

struct ApplicationContainerComponentData;

// All GCF components are subclasses of GCF::AbstractComponent class.
class ApplicationContainerComponent : public GCF::AbstractComponent
{
    Q_OBJECT

    // We have to make use of the GCF_DECLARE_COMPONENT macro
    // with the component class name as parameter to mark the
    // class as a GCF component class
    GCF_DECLARE_COMPONENT(ApplicationContainerComponent)

    public:
        // All GCF components must be singleton classes. They must
        // offer a public static instance() method that returns
        // pointer to the one and only instance of the class.
        static ApplicationContainerComponent& instance();

    // Destructor must be public
    ~ApplicationContainerComponent();

protected:
    // Following the singleton design pattern, the constructor
    // must be protected
    ApplicationContainerComponent();

    // The following functions must be implemented in every component
    // even if they dont do anything.
    void initializeComponent();
    void finalizeComponent();

    // If your component is "exposing" widgets, then it must implement
    // the following function
    QWidget* fetchWidget(const QString &completeName) const;

    // If your component is "exposing" objects, then it must implement
    // the following function
    QObject* fetchObject(const QString &completeName) const;

    // Since this class is the container component for your application,
    // you will have to implement the following functions in order
    // to support GCF's GUI merging mechanisms.
    void addChildWidget(QWidget *parent, QWidget *child, const QString &hint);
    void removeChildWidget(QWidget *parent, QWidget *child, const QString &hint);

private:
    ApplicationContainerComponentData* d;
};

#endif // APPLICATIONCONTAINERCOMPONENT_H
