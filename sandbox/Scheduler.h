#pragma once

#include <stdint.h>
#include <map>

class SchedulerObserver
{
public:
    virtual void onScheduleComplete( uint32_t _id ) = 0;
    virtual void onScheduleStop( uint32_t _id ) = 0;
};

class Scheduler
{
public:
    Scheduler();
    ~Scheduler();

public:
    uint32_t schedule( float _delay, bool _loop, SchedulerObserver * _observer );
    void stop( uint32_t _id );

    void update( float _time );

protected:
    uint32_t m_enumerator;

    struct Description
    {
        float delay;
        float time;

        SchedulerObserver * observer;

        bool loop;
        bool dead;
    };

    typedef std::map<uint32_t, Description> TMapSchedulers;
    TMapSchedulers m_schedulers;
};