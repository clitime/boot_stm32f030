#ifndef timer_hpp_
#define timer_hpp_

#include <cstdint>
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_bus.h"


class Timer {
private:
    enum class State {
        STOPPED, RUNNING, TIMEOUT
    } state;
public:
    explicit Timer(uint32_t ms = 0) : state(State::STOPPED), valuems(ms), counter(0) {};


    ~Timer() {}


    void tickinc() {
        if (state != State::RUNNING) return;

        if (counter < valuems) {
            counter++;
            return;
        }
        state = State::TIMEOUT;
    }


    void start() {
        state = State::RUNNING;
        counter = 0;
    }


    void reset(uint32_t ms = 0) {
        state = State::RUNNING;
        counter = 0;
        valuems = ms;
    }


    void stop() {
        state = State::STOPPED;
        counter = valuems;
    }


    bool istimeout() const {
        return state == State::TIMEOUT;
    }


    bool isrunning() const {
        return state == State::RUNNING;
    }


    bool isstopped() const {
        return state == State::STOPPED;
    }


private:
    uint32_t valuems;
    uint32_t counter;
};


class TimerHandler {
private:
    TimerHandler() : count(0) {}


    TimerHandler(const TimerHandler &);
    TimerHandler &operator=(const TimerHandler &);

    constexpr static uint8_t timer_amount = 4;
    Timer *timers[timer_amount];
    uint8_t count;
public:
    static TimerHandler *instance() {
        static TimerHandler self;
        return &self;
    }


    static void timInit() {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

        LL_TIM_SetPrescaler(TIM3, 48 - 1);
        LL_TIM_SetAutoReload(TIM3, 1000 - 1);
        LL_TIM_SetCounterMode(TIM3, LL_TIM_COUNTERDIRECTION_UP);
        LL_TIM_EnableARRPreload(TIM3);

        LL_TIM_EnableIT_UPDATE(TIM3);
        LL_TIM_EnableCounter(TIM3);

        NVIC_SetPriority(TIM3_IRQn, 0);
        NVIC_EnableIRQ(TIM3_IRQn);
    }


    bool registertimer(Timer *timer) {
        if (count == timer_amount) return false;

        timers[count++] = timer;
        return true;
    }


    void process() {
        for (uint8_t ix = 0; ix < count; ix++) {
            timers[ix]->tickinc();
        }
    }
};


#endif
