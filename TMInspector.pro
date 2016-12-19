#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T22:15:02
#
#-------------------------------------------------

QT       += core gui
QT	 += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TMInspector
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serversettings.cpp \
    sourcepacket.cpp \
    sourcepacketdatafieldheader.cpp \
    packetworker.cpp \
    packetstore.cpp \
    sqlworker.cpp \
    event.cpp \
    eventstore.cpp \
    store.cpp \
    objectview.cpp \
    objectfilterproxymodel.cpp \
    translationviewer.cpp \
    addtranslationdialog.cpp \
    animatedstandarditem.cpp \
    treeviewfilterproxymodel.cpp \
    packetcontentview.cpp \
    eventmodel.cpp \
    packetviewfilterproxymodel.cpp \
    translator.cpp \
    definitions.cpp \
    dumpsummarypacket.cpp

HEADERS  += mainwindow.h \
    serversettings.h \
    sourcepacket.h \
    sourcepacketdatafieldheader.h \
    packetworker.h \
    packetstore.h \
    sqlworker.h \
    event.h \
    eventstore.h \
    store.h \
    objectview.h \
    objectfilterproxymodel.h \
    translationviewer.h \
    addtranslationdialog.h \
    animatedstandarditem.h \
    treeviewfilterproxymodel.h \
    packetcontentview.h \
    definitions.h \
    custommodels.h \
    eventmodel.h \
    packetviewfilterproxymodel.h \
    translator.h \
    dumpsummarypacket.h

FORMS    += mainwindow.ui \
    serversettings.ui \
    objectview.ui \
    translationviewer.ui \
    addtranslationdialog.ui \
    packetcontentview.ui
