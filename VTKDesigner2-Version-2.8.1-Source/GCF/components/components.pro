TEMPLATE    = subdirs
CONFIG     += ordered
TARGET      = components

isEmpty(IGNORE_PROPERTY_EDITOR): SUBDIRS += PropertyEditor
isEmpty(IGNORE_UTILS): SUBDIRS += Utils
isEmpty(IGNORE_PROJECT_MANAGER): SUBDIRS += ProjectManager
isEmpty(IGNORE_MESSAGE_LOG): SUBDIRS += MessageLog
isEmpty(IGNORE_SCRIPT_EDITOR): SUBDIRS += ScriptEditor
isEmpty(IGNORE_MAIN_WINDOW): SUBDIRS += MainWindow
isEmpty(IGNORE_CLIPBOARD): SUBDIRS += Clipboard

