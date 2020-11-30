#pragma once

#include <QObject>
#include <QQuickItem>

#include "VideoReceiver.h"

class GStreamer {
public:
    static void initialize(int argc, char* argv[], int debuglevel, bool forceVAAPI = false, bool forceNVIDIA = false);
    static void* createVideoSink(QObject* parent, QQuickItem* widget);
    static void releaseVideoSink(void* sink);
    static VideoReceiver* createVideoReceiver(QObject* parent);
};
