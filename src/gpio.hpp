#ifndef GPIO_HPP_
#define GPIO_HPP_


#include "stm32f030x8.h"
#include <cstdint>
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"


class Gpio {
public:
    Gpio(GPIO_TypeDef *port,
         uint32_t pin,
         uint32_t mode = LL_GPIO_MODE_OUTPUT,
         uint32_t pull = LL_GPIO_PULL_NO,
         uint32_t speed = LL_GPIO_SPEED_FREQ_HIGH,
         uint32_t outtype = LL_GPIO_OUTPUT_PUSHPULL
    ) : port_(port), pin_(pin), mode_(mode) {
        if (port == GPIOA)
            LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
        else if (port == GPIOB)
            LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
        else if (port == GPIOC) {
            LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
        }

        configure(mode_, pull, speed, outtype);
        LL_GPIO_ResetOutputPin(port_, pin_);
    };

    enum class State {
        on, off
    };


    void set(State st) const {
        st == State::on ? LL_GPIO_SetOutputPin(port_, pin_) : LL_GPIO_ResetOutputPin(port_, pin_);
    }


    void toggle() const {
        LL_GPIO_TogglePin(port_, pin_);
    }


    bool get() const {
        if (mode_ == LL_GPIO_MODE_OUTPUT) return LL_GPIO_IsOutputPinSet(port_, pin_);
        else if (mode_ == LL_GPIO_MODE_INPUT) return LL_GPIO_IsInputPinSet(port_, pin_);
        return false;
    }


    void configure(uint32_t mode = LL_GPIO_MODE_OUTPUT,
                   uint32_t pull = LL_GPIO_PULL_NO,
                   uint32_t speed = LL_GPIO_SPEED_FREQ_HIGH,
                   uint32_t outtype = LL_GPIO_OUTPUT_PUSHPULL
    ) {
        mode_ = mode;
        LL_GPIO_SetPinMode(port_, pin_, mode_);
        LL_GPIO_SetPinPull(port_, pin_, pull);
        LL_GPIO_SetPinSpeed(port_, pin_, speed);
        LL_GPIO_SetPinOutputType(port_, pin_, outtype);
    }


private:
    GPIO_TypeDef *port_;
    uint32_t pin_;
    uint32_t mode_;

    Gpio(Gpio const &) = delete;
    Gpio &operator=(Gpio const &) = delete;
};


#endif //GPIO_HPP_
