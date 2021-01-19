#include "flashdata.hpp"
#include "stm32f030x8.h"


namespace FlashData {
    static inline void unlock() {
        if ((FLASH->CR & FLASH_CR_LOCK) == 0) return;
        /* Unlocking the program memory access */
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }


    static inline void lock() {
        FLASH->CR |= FLASH_CR_LOCK;
    }


    static inline void waitLastOperation() {
        while (FLASH->SR & FLASH_SR_BSY) {
            continue;
        }

        if (FLASH->SR & FLASH_SR_EOP) {
            FLASH->SR = FLASH_SR_EOP;
        }
    }


    static inline void writeHalfWorld(uint32_t addr, uint16_t data) {
        waitLastOperation();
        FLASH->CR |= FLASH_CR_PG;
        *(__IO uint16_t *) addr = data;
        waitLastOperation();
        FLASH->CR &= ~FLASH_CR_PG;
    }


    static inline void writeFullWorld(uint32_t addr, uint32_t data) {
        waitLastOperation();
        FLASH->CR |= FLASH_CR_PG;
        *(__IO uint16_t *) addr = data;
        waitLastOperation();
        addr += 2;
        *(__IO uint16_t *) addr = data >> 16;
        waitLastOperation();
        FLASH->CR &= ~FLASH_CR_PG;
    }


    void erasePage(uint32_t addr) {
        unlock();
        waitLastOperation();

        FLASH->CR |= FLASH_CR_PER;
        FLASH->AR = addr;
        FLASH->CR |= FLASH_CR_STRT;

        waitLastOperation();

        FLASH->CR &= ~FLASH_CR_PER;

        lock();
    }


    void write(uint32_t addr, uint16_t const *data, uint32_t len) {
        unlock();
        waitLastOperation();
        FLASH->CR |= FLASH_CR_PG;
        for (uint32_t i = 0; i != len; i++) {
            writeHalfWorld(addr, data[i]);
            addr += 2;
        }
        FLASH->CR &= ~(FLASH_CR_PG);
        lock();
    }
}
