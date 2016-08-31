#include "mainwindow.h"
#include <fstream>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QMutex>
#include <QMutexLocker>

static QMutex mutex;
FILE* file_l_out;

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

    qInstallMessageHandler(myMessageOutput);
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("./icon.png"));
    MainWindow w;

    w.show();

    return a.exec();
}
