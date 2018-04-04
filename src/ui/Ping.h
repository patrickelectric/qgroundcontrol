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
        qDebug() << "test";

        #ifdef Q_OS_OSX
            // macdeployqt file do not put stm32flash binary in the same folder of pingviewer
            static QString binPath = QCoreApplication::applicationDirPath() + "/ping/pingviewer/pingviewer.app/Contents/MacOS";
            static QString executable = "/pingviewer";
        #else
            static QString binPath = QCoreApplication::applicationDirPath();
            #ifdef Q_OS_WIN
                static QString executable = "\\pingviewer\\deploy\\pingviewer.exe";
            #else // Linux
                static QString executable = "/ping/pingviewer";
            #endif
        #endif

        connect(process, &QProcess::readyReadStandardOutput, this, [this] {
            qDebug() << process->readAllStandardOutput();
        });

        connect(process, &QProcess::errorOccurred, this, [](QProcess::ProcessError error) {
            qDebug() << "ProcessError:" << error;
        });

        connect(process, &QProcess::stateChanged, this, [](QProcess::ProcessState state) {
            qDebug() << "StateChanged:" << state;
        });

        #ifndef Q_OS_WIN
            qDebug() << QDir::toNativeSeparators(binPath + executable);
            process->startDetached(QDir::toNativeSeparators(binPath + executable));
        #else
            qDebug() << "\"" + QDir::toNativeSeparators(binPath + executable) + "\"";
            process->startDetached("\"" + QDir::toNativeSeparators(binPath + executable) + "\"");
        #endif
    }

    ~Ping()
    {
        process->kill();
        qDebug() << "PONG!";
    }

private:
    QProcess* process;
};