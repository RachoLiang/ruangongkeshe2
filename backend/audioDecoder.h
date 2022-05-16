#ifndef AUDIODECODER_H
#define AUDIODECODER_H

#include <QObject>

extern "C"
{
    #include <libswresample/swresample.h>
}

#include "avPacketQueue.h"

#endif // AUDIODECODER_H
