#-------------------------------------------------
#
# Project created by QtCreator 2016-04-19T21:59:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = newclass
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    parser.cpp \
    analize.cpp \
    vartypes.cpp

HEADERS  += mainwindow.h \
    parser.h \
    analize.h \
    vartypes.h

FORMS    += mainwindow.ui

QMAKE_CFLAGS += -save_temps -L/opt/lib

DISTFILES += \
    parser.s \
    vartypes.s \
    main.s \
    analize.s
