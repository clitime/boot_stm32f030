#ifndef APPLICATIONPROTOCOL_HPP_
#define APPLICATIONPROTOCOL_HPP_


#include <cstdint>

class AppProto {
    enum {
        CMD_IDLE_DATA = 0x00,
        CMD_GET_VERSION = 0x01,
        CMD_PROG_PAGE = 0x2,
        CMD_FINAL_PAGE = 0x3,
        CMD_PROG_FIRST_PAGE = 0x4,
        /* коды ответов */
        ANS_BAD_LEN = 0x1,
        ANS_BAD_CRC = 0x2,
        ANS_BAD_CMD = 0x3,
        ANS_ORDERPAGE_ERROR = 0x4,
        ANS_CMD_GOOD = 0x5,
        ANS_FINAL_PAGE_OK = 0x6
    };
    uint8_t *cmd_;
    uint8_t *ret_;
public:
    AppProto() = default;

    uint8_t operator()(uint8_t &cmd, uint8_t *data, uint32_t len);
private:
    uint8_t getVersion(uint8_t *data, uint32_t len);
    uint8_t progPage(uint8_t *data, uint32_t len);
    uint8_t progFirstPage(uint8_t *data, uint32_t len);
    uint8_t finalPage(uint8_t *data, uint32_t len);
    uint8_t error(uint8_t *data, uint32_t len);
};


#endif //APPLICATIONPROTOCOL_HPP_
