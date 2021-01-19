#ifndef queue_hpp_
#define queue_hpp_

#include <cstdint>
#include <array>

namespace MCU {
    template<class Type, std::size_t size_>
    class Queue {
        using array_t = std::array<Type, size_>;

        array_t buffer;
        typename array_t::iterator head = buffer.begin();
        typename array_t::iterator tail = buffer.begin();
        std::size_t length = 0;
    public:
        Queue() = default;
        Queue(const Queue &) = delete;
        Queue &operator=(const Queue &) = delete;


        void enqueue(Type const &item) {
            *tail = item;
            if (length != buffer.size()) ++length;

            if (tail == head && length == buffer.size()) {
                ++head;
                if (head == buffer.end()) head = buffer.begin();
            }

            if (++tail == buffer.end()) tail = buffer.begin();
        }


        Type dequeue(bool &ok) {
            if (empty()) {
                ok = false;
                return Type();
            }
            Type item = *head++;

            if (head == buffer.end()) head = buffer.begin();
            length--;
            ok = true;
            return item;
        }


        void reset() {
            length = 0;
            head = tail = buffer.begin();
        }


        bool empty() const { return length == 0; }


        bool full() const { return length == buffer.size(); }


        std::size_t capacity() const { return buffer.size(); };


        std::size_t size() const { return length; };
    };
}


#endif //queue_hpp_
