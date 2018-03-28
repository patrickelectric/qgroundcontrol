#pragma once

#include <QDebug>
#include <QProcess>
#include <QShowEvent>
#include <QWidget>
#include <QWindow>

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

        process->setEnvironment(QProcess::systemEnvironment());
        process->setProcessChannelMode(QProcess::MergedChannels);

        process->start("KDevelop.AppImage");
        connect(process, &QProcess::readyReadStandardOutput, this, [this] {
            qDebug() << process->readAllStandardOutput();
        });
    }

    ~Ping()
    {
        qDebug() << "PONG!";
    }

private:
    QProcess* process;
};