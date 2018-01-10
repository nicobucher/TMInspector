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

# For MacOS builds
QMAKE_MAC_SDK = macosx10.13

SOURCES += main.cpp\
        mainwindow.cpp \
    event.cpp \
    models/objectfilterproxymodel.cpp \
    animatedstandarditem.cpp \
    models/treeviewfilterproxymodel.cpp \
    models/eventmodel.cpp \
    models/packetviewfilterproxymodel.cpp \
    translator.cpp \
    definitions.cpp \
    packets/dumpsummarypacket.cpp \
    dialogs/addtranslationdialog.cpp \
    dialogs/serversettings.cpp \
    packets/sourcepacket.cpp \
    packets/sourcepacketdatafieldheader.cpp \
    stores/eventstore.cpp \
    stores/packetstore.cpp \
    stores/store.cpp \
    views/objectview.cpp \
    views/packetcontentview.cpp \
    views/translationviewer.cpp \
    workers/packetworker.cpp \
    workers/sqlworker.cpp \
    stores/dumpstore.cpp \
    dumpsummary.cpp \
    views/checksumview.cpp \
    packets/checksumpacket.cpp \
    models/dumpmodel.cpp \
    models/packetmodel.cpp \
    models/dumpviewfilterproxymodel.cpp \
    packets/variablepacket.cpp \
    workers/worker.cpp \
    packets/service206.cpp

HEADERS  += mainwindow.h \
    event.h \
    models/objectfilterproxymodel.h \
    animatedstandarditem.h \
    models/treeviewfilterproxymodel.h \
    definitions.h \
    models/custommodels.h \
    models/eventmodel.h \
    models/packetviewfilterproxymodel.h \
    translator.h \
    packets/dumpsummarypacket.h \
    dialogs/addtranslationdialog.h \
    dialogs/serversettings.h \
    packets/sourcepacket.h \
    packets/sourcepacketdatafieldheader.h \
    stores/eventstore.h \
    stores/packetstore.h \
    stores/store.h \
    views/objectview.h \
    views/packetcontentview.h \
    views/translationviewer.h \
    workers/packetworker.h \
    workers/sqlworker.h \
    stores/dumpstore.h \
    dumpsummary.h \
    views/checksumview.h \
    packets/checksumpacket.h \
    models/dumpmodel.h \
    models/packetmodel.h \
    helpers/variantptr.h \
    models/dumpviewfilterproxymodel.h \
    packets/variablepacket.h \
    workers/worker.h \
    interfaces/workerplugininterface.h \
    packets/service206.h

FORMS    += mainwindow.ui \
    dialogs/serversettings.ui \
    views/objectview.ui \
    views/translationviewer.ui \
    dialogs/addtranslationdialog.ui \
    views/packetcontentview.ui \
    views/checksumview.ui

DISTFILES += \
    LICENSE \
    README.md
