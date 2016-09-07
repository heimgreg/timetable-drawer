#-------------------------------------------------
#
# Project created by QtCreator 2016-08-10T20:02:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTimetableDrawer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    timetabledrawer.cpp \
    pdfwriter.cpp

HEADERS  += mainwindow.h \
    timetabledrawer.h \
    pdfwriter.h \
    event.h

FORMS    += mainwindow.ui

CONFIG   += c++11

LIBS     += -lhpdf

RESOURCES += \
    resources.qrc
