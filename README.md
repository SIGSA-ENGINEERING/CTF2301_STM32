# CTF2301_STM32
 STM32 Driver for Sensylink CTF2301 1 Local + 1 Remote Temperature Sensor with Integrated Fan Control

 -------

 Compatible Devices:

 CTF2301B Series


 ## How to use

 1. In the header file, change the included STM32 HAL Library if you are using the different MCUs. https://github.com/SIGSA-ENGINEERING/CTF2301_STM32/blob/46d516abc740bff5442bda0ccbeedea601f08b83/CTF2301.h#L37
 2. `#include "i2c.h"` can be commented out if your I2C is initalized in `main.c`
 3. Choose the correct I2C bus. Usually default is fine if your code is generated from CubeMX, BSP haven't been implemented yet. https://github.com/SIGSA-ENGINEERING/CTF2301_STM32/blob/46d516abc740bff5442bda0ccbeedea601f08b83/CTF2301.h#L70
 4. Change the I2C Handle if you are using different handles than default case. https://github.com/SIGSA-ENGINEERING/CTF2301_STM32/blob/46d516abc740bff5442bda0ccbeedea601f08b83/CTF2301.h#L76
 5. Include `CTF2301.h` in your project and enjoy!

## Settings

CTF2301 offer some advanced configuration for custom needs, if default settings isn't work for you, you can modify [here](https://github.com/SIGSA-ENGINEERING/CTF2301_STM32/blob/46d516abc740bff5442bda0ccbeedea601f08b83/CTF2301.h#L80) in the header file.
