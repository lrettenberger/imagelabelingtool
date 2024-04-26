#-------------------------------------------------
#
# Project created by QtCreator 2019-04-01T14:39:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = ImageLabelingTool
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

CONFIG += c++11 app_bundle

SOURCES += \
        colorselectiondialog.cpp \
        colorselectionwidget.cpp \
        imagecanvaswidget.cpp \
        imagescrollwidget.cpp \
        labelcolor.cpp \
        main.cpp \
        mainwindow.cpp \
        projectinfowidget.cpp \
        projectsettingsdialog.cpp

HEADERS += \
        colorbutton.h \
        colorselectiondialog.h \
        colorselectionwidget.h \
        imagecanvaswidget.h \
        imagescrollwidget.h \
        labelcolor.h \
        mainwindow.h \
        nowheelslider.h \
        projectinfowidget.h \
        projectsettingsdialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
