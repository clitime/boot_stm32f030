#ifndef USART_HPP
#define USART_HPP


#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_bus.h"

#include "stm32f0xx_ll_rcc_wrap.h"

#include "gpio.hpp"


#include <cstdint>
#include <cstring>

#include "queue.hpp"


extern "C"
void USART1_IRQHandler(void) __attribute__((interrupt));

template<size_t txsize, size_t rxsize>
class Usart {
    USART_TypeDef *usart_;
    Gpio txgpio;
    Gpio rxgpio;
    Gpio degpio;

    MCU::Queue<uint8_t, txsize> outqueue;
    MCU::Queue<uint8_t, rxsize> inqueue;
public:
    Usart(uint32_t baudrate = 9600);
    uint32_t sendbuffer(uint8_t const *data, uint32_t size);


    void sendbyte(uint8_t byte) const {
        LL_USART_TransmitData8(usart_, byte);
    }


    bool recvbyte(uint8_t byte) {
        inqueue.enqueue(byte);
        return true;
    }


    bool getbyte(uint8_t *data) {
        bool ok = false;
        *data = inqueue.dequeue(ok);
        return ok;
    }


    bool isrcvbyte() const {
        return !inqueue.empty();
    }


    void rcvclear() {
        inqueue.reset();
    }


private:
    Usart(Usart const &) = delete;
    Usart &operator=(Usart const &) = delete;

    friend void USART1_IRQHandler() __attribute__((interrupt));
};


template<size_t txsize, size_t rxsize>
Usart<txsize, rxsize>::Usart(uint32_t baudrate)
    : usart_(USART1),
      txgpio(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE, LL_GPIO_PULL_UP),
      rxgpio(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE, LL_GPIO_PULL_UP),
      degpio(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE, LL_GPIO_PULL_UP) {
    LL_APB1_GRP2_ForceReset(LL_APB1_GRP2_PERIPH_USART1);
    LL_APB1_GRP2_ReleaseReset(LL_APB1_GRP2_PERIPH_USART1);

    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_SYSCLK);
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

    LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_9, LL_GPIO_AF_1);
    LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_10, LL_GPIO_AF_1);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_1, LL_GPIO_AF_1);

    LL_USART_EnableDEMode(usart_);
    LL_USART_SetDEAssertionTime(usart_, 10);
    LL_USART_SetDEDeassertionTime(usart_, 10);

    LL_USART_Disable(usart_);
    LL_RCC_ClocksTypeDef clock;
    LL_RCC_GetSystemClocksFreq(&clock);
    LL_USART_SetBaudRate(usart_, clock.SYSCLK_Frequency, LL_USART_OVERSAMPLING_16, baudrate);
    LL_USART_SetDataWidth(usart_, LL_USART_DATAWIDTH_8B);
    LL_USART_SetStopBitsLength(usart_, LL_USART_STOPBITS_1);
    LL_USART_SetParity(usart_, LL_USART_PARITY_NONE);
    LL_USART_SetHWFlowCtrl(usart_, LL_USART_HWCONTROL_NONE);
    LL_USART_SetTransferDirection(usart_, LL_USART_DIRECTION_TX_RX);

    LL_USART_EnableIT_RXNE(usart_);

    NVIC_EnableIRQ(USART1_IRQn);
    LL_USART_Enable(usart_);
}


template<size_t txsize, size_t rxsize>
uint32_t Usart<txsize, rxsize>::sendbuffer(uint8_t const *data, uint32_t size) {
    uint8_t ix = 0;
    for (; !outqueue.full() && ix != size; ix++) {
        outqueue.enqueue(data[ix]);
    }

    LL_USART_EnableIT_TC(usart_);
    bool ok;
    sendbyte(outqueue.dequeue(ok));
    return size - ix;
}


#endif //USART_HPP
