#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>

#include "QGCDockWidget.h"

class Ping : public QGCDockWidget {
    Q_OBJECT
public:
    Ping(const QString& title, QAction* action, QWidget *parent)
        : QGCDockWidget(title, action, parent)
        , process(new QProcess())
    {
        _visible = false;
        qDebug() << "PING!";

        qDebug() << "envo" << process->processEnvironment().keys();
        process->setEnvironment(QStringList());
        qDebug() << "envo2" << process->processEnvironment().keys();
        qDebug() << "env" << QProcess::systemEnvironment();
        process->setProcessChannelMode(QProcess::MergedChannels);
        process->processEnvironment().clear();

        #ifdef Q_OS_OSX
            // macdeployqt file do not put stm32flash binary in the same folder of pingviewer
            static QString binPath = QCoreApplication::applicationDirPath() + "/ping/pingviewer/pingviewer.app/Contents/MacOS";
            static QString executable = "/pingviewer";
        #else
            static QString binPath = QCoreApplication::applicationDirPath();
            #ifdef  Q_OS_WIN
                static QString executable = "/pingviewer/deploy/pingviewer.exe";
            #else // Linux
                static QString executable = "/ping/pingviewer";
            #endif
        #endif

        qDebug() << (binPath + executable);
        process->startDetached(QDir::toNativeSeparators(binPath + executable));
        connect(process, &QProcess::readyReadStandardOutput, this, [this] {
            qDebug() << process->readAllStandardOutput();
        });
    }

    ~Ping()
    {
        process->kill();
        qDebug() << "PONG!";
    }

private:
    QProcess* process;
};