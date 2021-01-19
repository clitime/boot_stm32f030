#ifndef configuration_hpp_
#define configuration_hpp_


#include <cstdint>
#include "flashdata.hpp"


class Configuration {
    enum {
        FIRM = 0xa5, IS_FIRM = 0x55, BOOT = 0xff
    };
    static const uint32_t cfgAddr;

    Configuration();
    Configuration(const Configuration &) = delete;
    Configuration &operator=(const Configuration &) = delete;
public:
    static Configuration *instance() {
        static Configuration cfg;
        return &cfg;
    }


    void store();


    bool isBootMode() const { return boot.mode != FIRM; };


    bool isFirmStored() const { return boot.isFirm; };


    void setFirmIsIt() {
        boot.isFirm = true;
    }


    void resetFirmIsIt() {
        boot.isFirm = false;
    }


    void setModeFirm() {
        boot.mode = FIRM;
    }


    void setModeBoot() {
        boot.mode = BOOT;
    }


    static uint16_t calcCrc(const uint8_t *data, uint16_t len) {
        uint16_t crc = 0xffff;
        while (len--) {
            crc ^= *data++ << 8;
            for (uint8_t i = 0; i < 8; i++) {
                crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
            }
        }
        return crc;
    }


private:
    static constexpr uint16_t dataSize = FlashData::PAGE_SIZE / sizeof(uint16_t);
    union {
        uint16_t data[dataSize];
        struct {
            bool isFirm = false; // the firmware is stored in memory
            uint8_t mode = BOOT; // device mode: bootloader or firmware
        } boot;
    };

    static bool isCorrect();


};


#endif //configuration_hpp_
