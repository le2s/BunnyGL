#include "%ProjectName%.h"

GCF_DEFINE_COMPONENT(%ProjectName%)

struct %ProjectName%Data
{

};

%ProjectName%& %ProjectName%::instance()
{
    return *(GCF_CREATE_COMPONENT(%ProjectName%));
}

%ProjectName%::%ProjectName%()
{
    d = new %ProjectName%Data;
}

%ProjectName%::~%ProjectName%()
{
    delete d;
}

void %ProjectName%::initializeComponent()
{
    this->setComponentName("%ProjectName%");
    
    // TODO: Either set the GUIXML file from a resource file or 
    // allow GCF to load a GUIXML file from the GUIXML directory.
    // this->setGuiXmlFileName(":/%ProjectName%_GUIXML/%ProjectName%.xml");
}

void %ProjectName%::finalizeComponent()
{
}

void %ProjectName%::creationAnnounced()
{
}

QObject* %ProjectName%::fetchObject(const QString &completeName) const
{
    Q_UNUSED(completeName);
    return 0;
}

QWidget* %ProjectName%::fetchWidget(const QString &completeName) const
{
    Q_UNUSED(completeName);
    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(%ProjectName%)
