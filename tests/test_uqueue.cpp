#include "gtest/gtest.h"
#include "queue.hpp"


TEST(uqueueInit, simple) {
    MCU::Queue<std::uint32_t, 4> queue;

    ASSERT_EQ(4, queue.capacity());
    ASSERT_EQ(0, queue.size());
    ASSERT_TRUE(queue.empty());
    ASSERT_FALSE(queue.full());
}

TEST(uqueueInit, fullFill) {
    constexpr uint32_t size = 4;
    MCU::Queue<std::uint32_t, size> queue;

    for (uint32_t ix = 24; ix != 24 + size; ix++) {
        queue.enqueue(ix);
    }
    ASSERT_TRUE(queue.full());
    ASSERT_FALSE(queue.empty());
    bool ok = false;
    for (uint32_t ix = 24; ix != 24 + size; ix++) {
        ASSERT_EQ(ix, queue.dequeue(ok));
        ASSERT_TRUE(ok);
    }
    ASSERT_TRUE(queue.empty());
}

TEST(uqueueInit, fullFillUndeflow) {
    constexpr uint32_t size = 4;
    MCU::Queue<std::uint32_t, size> queue;

    for (uint32_t ix = 24; ix != 24 + 2*size; ix++) {
        queue.enqueue(ix);
    }
    ASSERT_TRUE(queue.full());
    ASSERT_FALSE(queue.empty());

    bool ok = false;
    for (uint32_t ix = 24 + size; ix != 24 + 2*size; ix++) {
        ASSERT_EQ(ix, queue.dequeue(ok));
        ASSERT_TRUE(ok);
    }
    ASSERT_EQ(std::uint32_t(), queue.dequeue(ok));
    ASSERT_FALSE(ok);
    ASSERT_TRUE(queue.empty());
}


TEST(uqueueInit, reset) {
    constexpr uint32_t size = 4;
    MCU::Queue<std::uint32_t, size> queue;
    queue.enqueue(25);
    ASSERT_FALSE(queue.empty());
    queue.reset();
    ASSERT_TRUE(queue.empty());
}
