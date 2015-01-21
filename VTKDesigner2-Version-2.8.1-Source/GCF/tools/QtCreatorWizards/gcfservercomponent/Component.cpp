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
    this->setGuiXmlFileName(":/%ProjectName%_GUIXML/%ProjectName%.xml");
    
    /**
    Ensure that the following XML elememt is inserted into the <Components> XML element of 
    $GCFDIR/bin/GCFApGCFApplicationServer-GUIXML/GCFApplicationServer.xml.
    
    <Component Name="%ProjectName%" Library="%TargetPath%/Binary/%ProjectName%" />
    */
}

void %ProjectName%::finalizeComponent()
{
}

QObject* %ProjectName%::fetchObject(const QString &completeName) const
{
    if(completeName == "%ProjectName%.Core")
        return const_cast<%ProjectName%*>(this);
        
    return 0;
}

GCF_EXPORT_COMPONENT_PLUGIN(%ProjectName%)
