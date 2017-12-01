#-------------------------------------------------
#
# Project created by QtCreator 2017-11-30T14:14:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opengl_midLesson05
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


SOURCES += main.cpp\
        mainwindow.cpp \
    databox/qmydatabox.cpp \
    modelbox/qmyrole.cpp \
    shader/myshadershadow.cpp \
    shader/qmyshaderdotlight.cpp \
    shader/qmyshaderlight.cpp \
    shader/qmyshadertnl.cpp \
    tool/mycamera.cpp \
    tool/programid.cpp \
    tool/program_p2_c4.cpp \
    xmlFile/qmyrapidxml.cpp \
    xmlFile/qmyrapidxml_faceandvertex.cpp \
    xmlFile/qmyrapidxml_perturbeduvs.cpp \
    qgleswidget.cpp \
    shader/qmyshadergetbox.cpp

HEADERS  += mainwindow.h \
    databox/qmydatabox.h \
    modelbox/qmyrole.h \
    shader/myshadershadow.h \
    shader/qmyshaderdotlight.h \
    shader/qmyshaderlight.h \
    shader/qmyshadertnl.h \
    tool/CELLMath.hpp \
    tool/mycamera.h \
    tool/programid.h \
    tool/program_p2_c4.h \
    xmlFile/qmyrapidxml.h \
    xmlFile/qmyrapidxml_faceandvertex.h \
    xmlFile/qmyrapidxml_perturbeduvs.h \
    xmlFile/rapidxml.hpp \
    qgleswidget.h \
    shader/qmyshadergetbox.h

FORMS    += mainwindow.ui
LIBS    += -lEGL -lfreeimage
