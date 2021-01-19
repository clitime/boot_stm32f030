#ifndef flashdata_hpp_
#define flashdata_hpp_

#include <cstdint>


namespace FlashData {
    enum {
        PAGES = 32, PAGE_SIZE = 1024
    };

    void erasePage(uint32_t addr);
    void write(uint32_t addr, uint16_t const *data, uint32_t len);
}


#endif //flashdata_hpp_
