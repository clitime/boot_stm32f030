#include "usart.hpp"


extern "C"
void USART1_IRQHandler(void) {
    extern Usart<32, 32> probe;

    if (LL_USART_IsActiveFlag_RXNE(probe.usart_)) {
        probe.recvbyte(LL_USART_ReceiveData8(probe.usart_));
    }

    if (LL_USART_IsActiveFlag_TC(probe.usart_)) {
        LL_USART_ClearFlag_TC(probe.usart_);

        if (LL_USART_IsActiveFlag_TXE(probe.usart_)) {
            if (!probe.outqueue.empty()) {
                bool ok;
                probe.usart_->TDR = probe.outqueue.dequeue(ok);
            } else {
                LL_USART_DisableIT_TC(probe.usart_);
            }
        }
    }
}
