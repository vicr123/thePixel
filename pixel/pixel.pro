#-------------------------------------------------
#
# Project created by QtCreator 2018-06-27T22:01:52
#
#-------------------------------------------------

QT       += core gui thelib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thepixel
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    screenshotwindow.cpp

HEADERS += \
        mainwindow.h \
    screenshotwindow.h

FORMS += \
        mainwindow.ui \
    screenshotwindow.ui

DISTFILES += \
    thepixel.desktop \
    thepixel.svg

unix {
    QMAKE_STRIP = echo
    target.path = /usr/bin

    appentry.path = /usr/share/applications
    appentry.files = thepixel.desktop

    icon.path = /usr/share/icons/hicolor/scalable/apps/
    icon.files = thepixel.svg

    INSTALLS += target appentry icon
}
