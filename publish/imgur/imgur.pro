TEMPLATE = lib
CONFIG += plugin
QT += widgets thelib network
TARGET = imgur

HEADERS += \
    imgurplugin.h \
    imgurui.h

SOURCES += \
    imgurplugin.cpp \
    imgurui.cpp

unix {
    target.path = /usr/share/thepixel/publish/

    INSTALLS += target
}

FORMS += \
    imgurui.ui

DISTFILES += \
    metadata.json
