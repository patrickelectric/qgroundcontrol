#pragma once

#include <QCoreApplication>
#include <QDebug>
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

        #ifdef Q_OS_OSX
            // macdeployqt file do not put stm32flash binary in the same folder of pingviewer
            static QString binPath = QCoreApplication::applicationDirPath() + "/../..";
            static QString executable = "/pingviewer.dmg";
        #else
            static QString binPath = QCoreApplication::applicationDirPath();
            #if Q_OS_WIN
                static QString executable = "/pingviewer.exe";
            #else // Linux
                static QString executable = "/pingviewer.AppImage";
            #endif
        #endif

        qDebug() << (binPath + executable);
        process->start(binPath + executable);
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