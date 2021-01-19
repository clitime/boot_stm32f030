#include "gtest/gtest.h"
#include "protocol.hpp"


TEST(transmition, validationFrom) {
    Protocol proto(0x51, [](uint8_t, uint8_t *, uint32_t){return 0;});
    uint8_t full_frame[] = {0x7E, 0x51, 0x00, 0x00, 0xa5, 0x62, 0x7E};

    for (uint8_t ix = 0; ix != sizeof(full_frame) - 1; ix++) {
        ASSERT_FALSE(proto.process(full_frame[ix]));
    }
    bool ret = proto.process(full_frame[sizeof(full_frame) - 1]);
    ASSERT_TRUE(ret);
    auto [data, len] = proto.getDataToSend();
    ASSERT_EQ(6, len);
    ASSERT_EQ(full_frame[0], data[0]);
    ASSERT_EQ(full_frame[1], data[1]);
    ASSERT_EQ(full_frame[2], data[2]);
}


TEST(transmition, validationFromStuffing) {
    Protocol proto(0x51, [](uint8_t, uint8_t *, uint32_t){return 0;});

    uint8_t full_frame[] = {0x7E, 0x00, 0x01, 0x7E, 0x7E, 0x51, 0x00, 0x00, 0xa5, 0x62, 0x7E};

    for (uint8_t ix = 0; ix != sizeof(full_frame) - 1; ix++) {
        ASSERT_FALSE(proto.process(full_frame[ix]));
    }
    ASSERT_TRUE(proto.process(full_frame[sizeof(full_frame) - 1]));
    auto [data, len] = proto.getDataToSend();
    ASSERT_EQ(6, len);
    ASSERT_EQ(full_frame[4], data[0]);
    ASSERT_EQ(full_frame[5], data[1]);
    ASSERT_EQ(full_frame[6], data[2]);
}


TEST(transmition, validationFromUncorrectAddr) {
    Protocol proto(51, [](uint8_t, uint8_t *, uint32_t){return 0;});

    uint8_t full_frame[] = {0x7E, 0x51, 0x00, 0x00, 0xa5, 0x62, 0x7E};

    for (uint8_t ix = 0; ix != sizeof(full_frame) - 1; ix++) {
        ASSERT_FALSE(proto.process(full_frame[ix]));
    }
    ASSERT_FALSE(proto.process(full_frame[sizeof(full_frame) - 1]));
}


TEST(transmition, validationFromUncorrect) {
    Protocol proto(0x51, [](uint8_t, uint8_t *, uint32_t){return 0;});

    uint8_t full_frame[] = {0x7E, 0x51, 0x00, 0x00, 0xa6, 0x62, 0x7E};

    for (uint8_t ix = 0; ix != sizeof(full_frame); ix++) {
        ASSERT_FALSE(proto.process(full_frame[ix]));
    }

    uint8_t full_frame3[] = {0x7E, 0x51, 0x01, 0x00, 0xa5, 0x62, 0x7E};

    for (uint8_t ix = 0; ix != sizeof(full_frame3); ix++) {
        ASSERT_FALSE(proto.process(full_frame3[ix]));
    }
}


TEST(transmition, cmdHandler) {
    auto cmdFn = [](uint8_t cmd, uint8_t *data, uint32_t len) {
        data[0] = 5;
        data[1] = 24;
        return 2;
    };

    Protocol proto(0x51, cmdFn);
    uint8_t full_frame[] = {0x7E, 0x51, 0x00, 0x00, 0xa5, 0x62, 0x7E};

    for (uint8_t ix = 0; ix != sizeof(full_frame) - 1; ix++) {
        ASSERT_FALSE(proto.process(full_frame[ix]));
    }

    ASSERT_TRUE(proto.process(full_frame[sizeof(full_frame) - 1]));

    auto [data, len] = proto.getDataToSend();

    ASSERT_EQ(8, len);
    ASSERT_EQ(full_frame[0], data[0]);
    ASSERT_EQ(full_frame[1], data[1]);
    ASSERT_EQ(full_frame[2], data[2]);
    ASSERT_EQ(5, data[3]);
    ASSERT_EQ(24, data[4]);
}
