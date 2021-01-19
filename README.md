## stm32f030c8_boot
template bootloader

bootloader fantasy based on HDLC protocol


### clone:
git clone --recursive https://github.com/clitime/stm32f030c8_template

### build:
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake ../

cmake --build ./ -- -j
