include(../../../../config.pri)

TEMPLATE = subdirs
SUBDIRS = 

win32 {
    system(copy /Y ..\\..\\..\\..\\tools\\GCFApplicationServer\\Scripts\\GCFMessage.js GCFMessage.js)
    system(copy /Y ..\\..\\..\\..\\tools\\GCFApplicationServer\\Scripts\\GCFApplicationServer.php .\\GCFApplicationServer.php)
}

unix {
    system(cp -f $$GCF_BASE/tools/GCFApplicationServer/Scripts/GCFMessage.js ./GCFMessage.js)
    system(cp -f $$GCF_BASE/tools/GCFApplicationServer/Scripts/GCFApplicationServer.php ./GCFApplicationServer.php)
}

