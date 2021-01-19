#include "gpio.hpp"
#include "timer.hpp"
#include "configuration.hpp"
#include "usart.hpp"
#include "bootutils.hpp"
#include "applicationprotocol.hpp"
#include "protocol.hpp"


static constexpr size_t recv_buf_size = 32;

Usart<32, recv_buf_size> probe;


static Timer redLedTimer(200);
static Timer resetMcu(2000);
static Timer waitRcvPkg(50);

extern bool isRestartMCU;


int main() {
    Gpio redLed(GPIOC, LL_GPIO_PIN_13);

    Configuration *cfg = Configuration::instance();

    if (!isBootloaderMode() && !cfg->isBootMode()) {
        redLed.set(Gpio::State::off);
        gotoApplication();
    }

    AppProto cmd;
    Protocol protocol(0x01, cmd);

    TimerHandler::timInit();

    TimerHandler *timerHandler = TimerHandler::instance();
    timerHandler->registertimer(&redLedTimer);
    timerHandler->registertimer(&resetMcu);
    timerHandler->registertimer(&waitRcvPkg);

    if (cfg->isFirmStored())
        resetMcu.start();

    while (1) {
        if (!redLedTimer.isrunning()) {
            redLedTimer.start();
            redLed.toggle();
        }

        if (resetMcu.istimeout()) {
            resetMcu.reset(1000);
            break;
        }

        if (probe.isrcvbyte()) {
            waitRcvPkg.start();
            uint8_t byte;
            probe.getbyte(&byte);

            if (protocol.process(byte)) {
                auto[data, len] = protocol.getDataToSend();
                probe.sendbuffer(data, len);
            }
        }

        if (isRestartMCU) {
            if (cfg->isFirmStored()) {
                cfg->setModeFirm();
                cfg->store();
            }

            redLed.set(Gpio::State::off);
            __disable_irq();
            NVIC_SystemReset();
        }
//            resetMcu.reset(1000);
    }

//    while (resetMcu.isrunning()) {continue;}
//    if (cfg->isFirmStored()) {
//        cfg->setModeFirm();
//        cfg->store();
//    }
//
//    redLed.set(Gpio::State::off);
//    __disable_irq();
//    NVIC_SystemReset();

    return 0;
}
