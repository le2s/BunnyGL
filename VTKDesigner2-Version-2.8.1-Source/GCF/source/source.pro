TEMPLATE        = lib
CONFIG          += shared

CONFIG(debug, debug|release) {
    win32: TARGET = GCFd
    unix: TARGET = GCFd2
    mac: TARGET = GCFd2
} else {
    win32: TARGET = GCF
    unix: TARGET = GCF2
    mac: TARGET = GCF2
}

BASE_DIR = ../
!isEmpty(ROOT_DIR): BASE_DIR = $$ROOT_DIR

DLLDESTDIR = $$BASE_DIR/bin
DESTDIR = $$BASE_DIR/lib

DEFINES += VCL_MAKE_DLL
RESOURCES = source.qrc
INCLUDEPATH += . ../include
QT += xml network script
mac: CONFIG -= lib_bundle

###################### Version Information #########################

VERSION         = 2.6.1

################ Support for ActiveX and KParts ####################

# August 12, 2008. @Akademy 2008, Akonadi/KOffice Den
# Support for ActiveX in GCF
win32 {
    isEmpty(GCF_DISABLE_ACTIVEX) {
        exists( $$(QTDIR)/src/activeqt/container/container.pro ) {
            CONFIG += qaxcontainer
            DEFINES += GCF_ACTIVEQT_AVAILABLE
        #    message("Building GCF with ActiveX support")
        }
    }
}

# Support for KParts in GCF
isEmpty(GCF_DISABLE_KPARTS) {
    include(../kde4.pri)
    !isEmpty(KDE4_INCLUDE_DIR) {
        !isEmpty(KDE4_LIB_DIR) {
            INCLUDEPATH += $$KDE4_INCLUDE_DIR
            LIBS        += -L$$KDE4_LIB_DIR -lkparts
            DEFINES     += GCF_KPARTS_AVAILABLE
            message("Building GCF with KParts support")
        }
    }
}

INCLUDEPATH    += core ipc menustrip updater qml

SOURCES         =   \                                        # GCF - Core
                    core/AbstractComponent.cpp \
                    core/ComponentFactory.cpp \
                    core/ComponentGui.cpp \
                    core/ComponentLoaderComponent.cpp \
                    core/ComponentSettings.cpp \
                    core/ConfigurationDialog.cpp \
                    core/ConfigurationDialogComponent.cpp \
                    core/DataStore.cpp \
                    core/Logger.cpp \
                    core/ObjectIterator.cpp \
                    core/DomAccess.cpp \
                    core/Application.cpp \
                    \                                        # GCF - IPC
                    ipc/AppAccessPoint.cpp \
                    ipc/AppAccessPointComponent.cpp \
                    ipc/Message.cpp \
                    ipc/MethodInvocationHelper.cpp \
                    ipc/RemoteAppAccessPoint.cpp \
                    ipc/RemoteAppDiscovery.cpp \
                    ipc/RemoteObject.cpp \
                    \                                        # GCF - MenuStrip
                    menustrip/ActionContainerWidget.cpp \
                    menustrip/ActionDocumentMapper.cpp \
                    menustrip/GenericSignalSpy.cpp \
                    menustrip/Menu.cpp \
                    menustrip/SignalMultiplexer.cpp \
                    \                                        # GCF - Updater
                    updater/FileDownloader.cpp \
                    updater/UpdateDownloaderThread.cpp \
                    updater/UpdateFinderThread.cpp \
                    updater/UpdateInfo.cpp \
                    updater/UpdaterComponent.cpp \
                    updater/UpdateXMLParser.cpp \
                    updater/UpdaterConfigPage.cpp \
                    updater/IconCache.cpp \
                    updater/UpdateSelectionDialog.cpp \
    qml/QMLComponent.cpp \
    core/ObjectList.cpp \
    core/VariantComparisonHelper.cpp \
    error/ErrorMessage.cpp \
    error/ErrorDump.cpp \
    error/ErrorMessageDialog.cpp

HEADERS         =   \                                        # GCF - Core
                    ../include/GCF/AbstractComponent.h \
                    ../include/GCF/ComponentFactory.h \
                    ../include/GCF/ComponentGui.h \
                    ../include/GCF/ComponentLoaderComponent.h \
                    ../include/GCF/ComponentSettings.h \
                    ../include/GCF/ConfigurationDialog.h \
                    ../include/GCF/ConfigurationDialogComponent.h \
                    ../include/GCF/DataStore.h \
                    core/ComponentFactoryTable \
                    ../include/GCF/Logger.h \
                    ../include/GCF/ObjectIterator.h \
                    ../include/GCF/DomAccess.h \
                    ../include/GCF/Application.h \
                    ../include/GCF/ObjectList.h \
                    \                                        # GCF - IPC
                    ../include/GCF/AppAccessPoint.h \
                    ../include/GCF/AppAccessPointComponent.h \
                    ../include/GCF/Message.h \
                    ../include/GCF/MethodInvocationHelper.h \
                    ../include/GCF/RemoteAppAccessPoint.h \
                    ../include/GCF/RemoteAppDiscovery.h \
                    ../include/GCF/RemoteObject.h \
                    ipc/RemoteAppAccessPoint_p.h \
                    ipc/AppAccessPoint_p.h \
                    \                                        # GCF - MenuStrip
                    ../include/GCF/ActionContainerWidget.h \
                    ../include/GCF/ActionDocumentMapper.h \
                    ../include/GCF/GenericSignalSpy.h \
                    ../include/GCF/Menu.h \
                    ../include/GCF/SignalMultiplexer.h \
                    \                                        # GCF - Updater
                    updater/FileDownloader.h \
                    updater/UpdateDownloaderThread.h \
                    updater/UpdateFinderThread.h \
                    ../include/GCF/UpdateInfo.h \
                    ../include/GCF/UpdaterComponent.h \
                    updater/UpdateXMLParser.h \
                    updater/UpdaterConfigPage.h \
                    updater/IconCache.h \
                    updater/UpdateSelectionDialog.h \
    qml/QMLComponent.h \
    ../include/GCF/VariantComparisonHelper.h \
    error/IErrorMessageHandler.h \
    error/ErrorMessage.h \
    error/ErrorDump.h \
    error/ErrorMessageDialog.h \
    ../include/GCF/Pointer.h

FORMS           =   \                                        # GCF - Core
                    core/AddPropertyDialog.ui \
                    core/ComponentLoaderConfigPage.ui \
                    core/ComponentPropertyEditorPage.ui \
                    core/ConfigurationDialog.ui \
                    \                                        # GCF - IPC
                    ipc/AddRemoteAppDialog.ui \
                    ipc/AppAccessPointConfigPage.ui \
                    \                                        # GCF - Updater
                    updater/UpdaterConfigPage.ui \
                    updater/UpdateSelectionDialog.ui \
    error/ErrorMessageDialog.ui


# GCF -QML
lessThan(QT_MAJOR_VERSION, 4) | lessThan(QT_MINOR_VERSION, 7) {
    message("QML Support will not be built into GCF")
} else {
    QT         += declarative

    INCLUDEPATH    += qml

    HEADERS += \
               ../include/GCF/DeclarativeView.h

    SOURCES += \
               qml/DeclarativeView.cpp

}
