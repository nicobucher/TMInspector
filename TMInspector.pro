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
    event.cpp \
    objectfilterproxymodel.cpp \
    animatedstandarditem.cpp \
    treeviewfilterproxymodel.cpp \
    eventmodel.cpp \
    packetviewfilterproxymodel.cpp \
    translator.cpp \
    definitions.cpp \
    packets/sourcepacket.cpp \
    dialogs/addtranslationdialog.cpp \
    dialogs/serversettings.cpp \
    stores/eventstore.cpp \
    stores/packetstore.cpp \
    stores/store.cpp \
    views/objectview.cpp \
    views/packetcontentview.cpp \
    views/translationviewer.cpp \
    workers/packetworker.cpp \
    workers/sqlworker.cpp \
    packets/sourcepacketdatafieldheader.cpp

HEADERS  += mainwindow.h \
    event.h \
    objectfilterproxymodel.h \
    animatedstandarditem.h \
    treeviewfilterproxymodel.h \
    definitions.h \
    custommodels.h \
    eventmodel.h \
    packetviewfilterproxymodel.h \
    translator.h \
    packets/sourcepacket.h \
    dialogs/addtranslationdialog.h \
    dialogs/serversettings.h \
    stores/eventstore.h \
    stores/packetstore.h \
    stores/store.h \
    views/objectview.h \
    views/packetcontentview.h \
    views/translationviewer.h \
    workers/packetworker.h \
    workers/sqlworker.h \
    packets/sourcepacketdatafieldheader.h

FORMS    += mainwindow.ui \
    dialogs/serversettings.ui \
    views/objectview.ui \
    views/translationviewer.ui \
    dialogs/addtranslationdialog.ui \
    views/packetcontentview.ui
