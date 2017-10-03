#include "gameloop.h"


GameLoop::GameLoop(unsigned int fps, QObject *parent) :
    QObject(parent),
    m_fps(fps),
    m_timer(),
    m_callback()
{}

unsigned int GameLoop::fps() const
{
    return m_fps;
}

void GameLoop::setCallback(const std::function<void ()> &callback)
{
    m_callback = callback;
}

void GameLoop::run()
{
    m_timer.start(1000 / m_fps, this);
}

void GameLoop::timerEvent(QTimerEvent *)
{
    if (m_callback) {
        m_callback();
    }
}
