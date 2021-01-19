#include "configuration.hpp"
#include "stm32f030x8.h"


const uint32_t Configuration::cfgAddr = FLASH_BASE + (FlashData::PAGES - 1) * FlashData::PAGE_SIZE;


Configuration::Configuration() {
    if (!isCorrect()) {
        store();
    } else {
        boot.isFirm = *reinterpret_cast<const uint8_t *>(cfgAddr) == IS_FIRM ? true : false;
        boot.mode = *reinterpret_cast<const uint8_t *>(cfgAddr + 1);
    }
}


void Configuration::store() {
    FlashData::erasePage(cfgAddr);
    data[dataSize - 2] = calcCrc(reinterpret_cast<const uint8_t *>(data), FlashData::PAGE_SIZE - 4);
    FlashData::write(cfgAddr, data, dataSize);
}


bool Configuration::isCorrect() {
    uint16_t crc = calcCrc(reinterpret_cast<const uint8_t *>(cfgAddr), FlashData::PAGE_SIZE - 4);
    uint16_t crcStored = *reinterpret_cast<const uint16_t *>(cfgAddr + FlashData::PAGE_SIZE - 4);
    return crc == crcStored;
}
