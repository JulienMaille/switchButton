TEMPLATE = lib
TARGET   = $$qtLibraryTarget($$TARGET)
CONFIG  += plugin

!build_pass: message( + $$TARGET )

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/designer

HEADERS     = SwitchButtonPlugin.h
SOURCES     = SwitchButtonPlugin.cpp
RESOURCES   = designericons.qrc

# install
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS   += target

include(switchbutton.pri)