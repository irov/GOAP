#pragma once

#include "GOAP/Config.h"
#include "GOAP/Vector.h"
#include "GOAP/Factorable.h"
#include "GOAP/IntrusivePtr.h"

class SchedulerObserver
    : public GOAP::Factorable
{
public:
    virtual void onScheduleComplete( uint32_t _id ) = 0;
    virtual void onScheduleStop( uint32_t _id ) = 0;
};

typedef GOAP::IntrusivePtr<SchedulerObserver> SchedulerObserverPtr;

class Scheduler
    : public GOAP::Factorable
{
public:
    Scheduler();
    ~Scheduler();

public:
    uint32_t schedule( float _delay, bool _loop, const SchedulerObserverPtr & _observer );
    void stop( uint32_t _id );

    void update( float _time );

protected:
    uint32_t m_enumerator;

    struct Description
    {
        uint32_t id;

        float delay;
        float time;

        SchedulerObserverPtr observer;

        bool loop;
        bool dead;
    };

    typedef GOAP::Vector<Description> VectorSchedulers;
    VectorSchedulers m_schedulersAdd;
    VectorSchedulers m_schedulers;
};

typedef GOAP::IntrusivePtr<Scheduler> SchedulerPtr;