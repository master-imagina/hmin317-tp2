#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <functional>

#include <QBasicTimer>
#include <QObject>


class GameLoop : public QObject
{
public:
    explicit GameLoop(unsigned int fps, QObject *parent = nullptr);

    unsigned int fps() const;

    void setCallback(const std::function<void ()> &callback);

    void run();

protected:
    void timerEvent(QTimerEvent *) override;

private:
    unsigned int m_fps;
    QBasicTimer m_timer;

    std::function<void ()> m_callback;
};

#endif // GAMELOOP_H
