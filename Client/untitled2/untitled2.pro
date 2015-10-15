#-------------------------------------------------
#
# Project created by QtCreator 2015-09-19T00:39:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled2
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    clientFramehandler.cpp \
    clientTransmitHandler.cpp \
    gameClient.cpp \
    playerClient.cpp \
    tcpclient.cpp \
    Tile_client.cpp

HEADERS  += \
    mainwindow.h \
    clientFramehandler.h \
    clientTransmitHandler.h \
    gameClient.h \
    playerClient.h \
    tcpclient.h \
    Tile_client.h

FORMS    += \
#    mainwindow - Copy.ui
    mainwindow.ui

LIBS += -lws2_32

DISTFILES +=

CONFIG += c++14
