#-------------------------------------------------
#
# Project created by QtCreator 2016-08-10T20:02:50
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTimetableDrawer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    timetabledrawer.cpp \
    pdfwriter.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
    timetabledrawer.h \
    pdfwriter.h \
    event.h \
    helpdialog.h

FORMS    += mainwindow.ui \
    helpdialog.ui

CONFIG   += c++11

LIBS     += -lhpdf

RESOURCES += \
    resources.qrc
