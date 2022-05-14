#include "backend/avPacketQueue.h"

#pragma comment(lib, "version.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "oleaut32.lib")

AvPacketQueue::AvPacketQueue()
{
    mutex   = SDL_CreateMutex();
    cond    = SDL_CreateCond();
}

void AvPacketQueue::enqueue(AVPacket *packet)
{
    SDL_LockMutex(mutex);

    queue.enqueue(*packet);

    SDL_CondSignal(cond);
    SDL_UnlockMutex(mutex);
}

void AvPacketQueue::dequeue(AVPacket *packet, bool isBlock)
{
    SDL_LockMutex(mutex);
    while (1) {
        if (!queue.isEmpty()) {
            *packet = queue.dequeue();
            break;
        } else if (!isBlock) {
            break;
        } else {
            SDL_CondWait(cond, mutex);
        }
    }
    SDL_UnlockMutex(mutex);
}

void AvPacketQueue::empty()
{
    SDL_LockMutex(mutex);
    while (queue.size() > 0) {
        AVPacket packet = queue.dequeue();
        av_packet_unref(&packet);
    }

    SDL_UnlockMutex(mutex);
}

bool AvPacketQueue::isEmpty()
{
    return queue.isEmpty();
}

int AvPacketQueue::queueSize()
{
    return queue.size();
}
