#ifndef AUDIO_H
#define AUDIO_H

#endif // AUDIO_H
#include <QMediaPlayer>
#include "captions.h"
#include <QString>
#include <QList>

class audio:public captions
{
public:
    audio() {}

private:
    QMediaPlayer* player;

};
