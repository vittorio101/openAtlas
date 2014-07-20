 #-------------------------------------------------
#
# Project created by QtCreator 2014-04-28T16:55:15
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openAtlas-dev
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    firststart.cpp \
    databasemanager.cpp \
    createconnection.cpp \
    typewizard.cpp \
    preferences.cpp \
    tree_editor.cpp

HEADERS  += mainwindow.h \
    login.h \
    firststart.h \
    databasemanager.h \
    createconnection.h \
    version.h \
    typewizard.h \
    preferences.h \
    tree_editor.h

FORMS    += mainwindow.ui \
    login.ui \
    firststart.ui \
    createconnection.ui \
    typewizard.ui \
    preferences.ui \
    tree_editor.ui

RESOURCES += \
    Icons.qrc

# link openAtlas to marble library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lmarblewidget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lmarblewidget
else:symbian: LIBS += -lmarblewidget
else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -lmarblewidget

INCLUDEPATH += $$PWD/../../../../usr/include/marble
DEPENDPATH += $$PWD/../../../../usr/include/marble
