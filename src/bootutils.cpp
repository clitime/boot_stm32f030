#include "bootutils.hpp"
#include "gpio.hpp"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_rcc_wrap.h"


/**
 * PA0 - input
 * PA1 - output
 * if PA0 is closed to PA1 then bootloader mode
 */
bool isBootloaderMode() {
    Gpio in(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
    Gpio out(GPIOA, LL_GPIO_PIN_1);

    bool res = true;

    do {
        out.set(Gpio::State::on);
        if (!in.get()) {
            res = false;
            break;
        }
        out.set(Gpio::State::off);
        if (in.get()) {
            res = false;
            break;
        }
    } while (0);
    in.configure();
    return res;
}


static volatile uint32_t *vectorTable = (volatile uint32_t *) 0x20000000;


void gotoApplication() {
#define USER_FLASH_FIRST_PAGE_ADDRESS (0x08003000UL)
    static uint32_t jumpAddress;
    typedef void (*pFunction)(void);
    pFunction jump_to_application;

    __disable_irq();
    LL_RCC_DeInit();

    SysTick->CTRL = 0;
    SysTick->VAL = 0;

    for (uint8_t i = 0; i < 48; i++) {
        vectorTable[i] = *(__IO uint32_t *) (USER_FLASH_FIRST_PAGE_ADDRESS + (i << 2));
    }
    LL_SYSCFG_SetRemapMemory(LL_SYSCFG_REMAP_SRAM);

    //Jump to user application (reset handler)
    jumpAddress = *(volatile uint32_t *) (USER_FLASH_FIRST_PAGE_ADDRESS + 4);
    jump_to_application = (pFunction) jumpAddress;
    __set_MSP(*(volatile uint32_t *) USER_FLASH_FIRST_PAGE_ADDRESS);
    jump_to_application();
}
