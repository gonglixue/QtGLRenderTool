#-------------------------------------------------
#
# Project created by QtCreator 2020-02-15T22:12:18
#
#-------------------------------------------------

# QT       += core gui opengl
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtGLRenderTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
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
    centerglwidget.cpp \
    mycamera.cpp \
    mymesh.cpp \
    myshader.cpp \
    texture2d.cpp \
    littlethings.cpp

HEADERS += \
        mainwindow.h \
    centerglwidget.h \
    mycamera.h \
    mymesh.h \
    myshader.h \
    texture2d.h \
    littlethings.h

FORMS += \
        mainwindow.ui

# LIBS += -lopengl32 -lglu32

#RESOURCES += \
#    res.qrc

DISTFILES += \
    readme.md
