#include "mainwindow.h"
#include <fstream>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QMutex>
#include <QMutexLocker>
#include <QtPlugin>

static QMutex mutex;
FILE* file_l_out;

// Permanent Settings
QSettings settings;

// Stores
PacketStore myPacketStore;
PacketStore mySqlPacketStore;
EventStore myEventStore;
EventStore mySqlEventStore;
DumpStore myDumpStore;

// Global Translators
SPIDTranslator mySPIDTranslator;
PICTranslator myPICTranslator;
PITranslator myPITranslator;
ObjectTranslator myObjectTranslator;
EventTranslator myEventTranslator;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&mutex);
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(file_l_out, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(file_l_out, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(file_l_out, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(file_l_out, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fprintf(file_l_out, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        fflush(stderr);
        fflush(file_l_out);
        abort();
    }
    fflush(stderr);
    fflush(file_l_out);
}

int main(int argc, char *argv[])
{
    file_l_out = fopen("log.txt", "w+");

//    fprintf(stderr,"argc=%i\n", argc);
//    fprintf(stderr,"argv=%s\n", *argv);

    // Initialize Translators
    mySPIDTranslator.Translator::loadHash();
    myPICTranslator.Translator::loadHash();
    myPITranslator.Translator::loadHash();
    myObjectTranslator.Translator::loadHash();
    myEventTranslator.Translator::loadHash();

    // Initial Settings
    if (settings.value("time_fmt").isNull()) {
        settings.setValue("time_fmt", "yyyy.MM.dd - hh:mm:ss:zzz");
    }
    myPacketStore.setModelTimestampFmt(settings.value("time_fmt").toString());
    mySqlPacketStore.setModelTimestampFmt(settings.value("time_fmt").toString());
    myEventStore.setModelTimestampFmt(settings.value("time_fmt").toString());
    mySqlEventStore.setModelTimestampFmt(settings.value("time_fmt").toString());
    myDumpStore.setModelTimestampFmt(settings.value("time_fmt").toString());

    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("./icon.png"));
    MainWindow w;

    w.show();

    return a.exec();
}
