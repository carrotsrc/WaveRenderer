#-------------------------------------------------
#
# Project created by QtCreator 2016-03-13T19:32:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlacView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    waverenderer.cpp \
    waveport.cpp

HEADERS  += mainwindow.hpp \
    waverenderer.hpp \
    waveport.hpp

FORMS    += mainwindow.ui
