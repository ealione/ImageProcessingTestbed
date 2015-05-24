#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T15:56:24
#
#-------------------------------------------------

QT       += core gui script scripttools
qtHaveModule(printsupport): QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcessingTestbed
TEMPLATE = app

VERSION = 0.0.1

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/historycommands.cpp \
    src/Algorithms/alg_defaulteffects.cpp \
    src/variablesettings.cpp \
    src/settingsdialog.cpp \
    ScriptEditor/scripteditor.cpp \
    ScriptEditor/highlighter.cpp \
    src/scriptrunner.cpp \
    src/scriptmanager.cpp \
    src/scriptsaver.cpp

HEADERS  += src/mainwindow.h \
    src/enums.h \
    src/historycommands.h \
    src/Algorithms/alg_defaulteffects.h \
    src/variablesettings.h \
    src/settingsdialog.h \
    ScriptEditor/scripteditor.h \
    ScriptEditor/highlighter.h \
    src/scriptrunner.h \
    src/scriptingconstants.h \
    src/scriptmanager.h \
    src/scriptsaver.h

RESOURCES = src/icons.qrc \
    ScriptEditor/completer.qrc

INCLUDEPATH += $$PWD/src/Algorithms/

#OTHER_FILES += \
#    etc/* \
#    ext/* \
#    logs/*

include(DoubleViewport/doubleviewport.pri)

#if mac
#ICON = logo.icns
#if win
#win32:RC_ICONS += logo.ico

FORMS += \
    src/settingsdialog.ui \
    src/scriptsaver.ui

QMAKE_CXXFLAGS += -std=c++11 # I use lamdas instead of mapping. For msvc I think it is -std=c++0x.
