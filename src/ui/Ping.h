#pragma once

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>

#include "QGCDockWidget.h"

Q_DECLARE_LOGGING_CATEGORY(PingCategory)

class Ping : public QGCDockWidget {
    Q_OBJECT
public:
    Ping(const QString& title, QAction* action, QWidget *parent)
        : QGCDockWidget(title, action, parent)
        , process(new QProcess())
    {
        _visible = false;
        process->setEnvironment(QStringList());
        process->setProcessChannelMode(QProcess::MergedChannels);

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
            qCDebug(PingCategory) << process->readAllStandardOutput();
        });

        connect(process, &QProcess::errorOccurred, this, [](QProcess::ProcessError error) {
            qCDebug(PingCategory) << "ProcessError:" << error;
        });

        connect(process, &QProcess::stateChanged, this, [](QProcess::ProcessState state) {
            qCDebug(PingCategory) << "StateChanged:" << state;
        });

        #ifndef Q_OS_WIN
            qCDebug(PingCategory) << QDir::toNativeSeparators(binPath + executable);
            process->startDetached(QDir::toNativeSeparators(binPath + executable));
        #else
            qCDebug(PingCategory) << "\"" + QDir::toNativeSeparators(binPath + executable) + "\"";
            process->startDetached("\"" + QDir::toNativeSeparators(binPath + executable) + "\"");
        #endif
    }

    ~Ping()
    {
        process->kill();
    }

private:
    QProcess* process;
};