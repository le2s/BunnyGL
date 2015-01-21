#ifndef %ProjectName:u%_H
#define %ProjectName:u%_H

#include <GCF/AbstractComponent>
#include <GCF/WSMacros>

struct %ProjectName%Data;
class %ProjectName% : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(%ProjectName%)

public:
    static %ProjectName%& instance();
    ~%ProjectName%();

protected:
    %ProjectName%();
    void initializeComponent();
    void finalizeComponent();
    QObject* fetchObject(const QString &completeName) const;

private:
    %ProjectName%Data* d;
};

#endif // %ProjectName:u%_H
