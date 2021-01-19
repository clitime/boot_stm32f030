#include "timer.hpp"


extern "C"
void TIM3_IRQHandler(void) {
    if (LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
        LL_TIM_ClearFlag_UPDATE(TIM3);

        TimerHandler *timerHandler = TimerHandler::instance();
        timerHandler->process();
    }
}
