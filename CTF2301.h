/*

  Sensylink CTF2301 Library for STM32 HAL

  MIT License

  Copyright (c) 2024 Weiwei Su, Ausinter Technologies Co., Ltd.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  
  //
  Revision History

  9/24/2024 - V0.1 - Initial Version
  
 */

#ifndef INC_SENSYLINK_H_
#define INC_SENSYLINK_H_

#ifdef __cplusplus
 extern "C" {
#endif

// Include your STM32's HAL Library here

#include "stm32g0xx_hal.h"

// Include the header file where your I2C handle is declared

#include "i2c.h"

#warning "Functions require I2C communications here uses HAL_Delay(), if you are using FreeRTOS and want to call these functions in a task, you should overwrite HAL_Delay() to use OS Tick instead."

// CTF2301 Exported Constants

// ERROR CODES
#define CTF2301_OK                       0
#define CTF2301_ERROR                   -1
#define CTF2301_ERROR_COMM              -2      // I2C Communication Error
#define CTF2301_ERROR_ID                -3      // Device ID mismatch, someone uses the different chip
#define CTF2301_ERROR_NOT_READY         -4      // Device is not ready

#define CTF2301_STEP_DIE_REV_ID          0x01
#define CTF2301_MANUFACTURER_ID          0x59

#define CTF2301_MX_I2C_BUS               1
#define CTF2301_BSP_I2C_BUS              2

// CTF2301 Fixed I2C Address
#define configDEVICE_CTF2301_I2C_ADDR          0x4C
// CTF2301 Alert Response Address
#define configDEVICE_ALERT_RESPONSE_ADDR       0x19

// Select the peripheral driver to use for the CTF2301
// If you are using CubeMX, you might be using BSP Driver instead of MX Driver
// for I2C or SPI if you have used other Sensor Libraries from STMicroelectronics. 
// Then it is better to use BSP Driver for PCM51xx as well.

#define configUSE_PERIPHERIAL_DRIVER_CTF2301           CTF2301_MX_I2C_BUS  // or BSP_I2C_BUS

#if ( configUSE_PERIPHERIAL_DRIVER_CTF2301 == CTF2301_MX_I2C_BUS )

#warning "Please make sure you have included the correct I2C Handle here"

#define CTF2301_I2C_HANDLE hi2c2 // Change this to your I2C Handle

#endif // configUSE_PERIPHERIAL_DRIVER

// Device Control Mode
// Default will be set at Auto-Temp Mode, uncomment below to enable Manual Direct-DCY Mode

#define configUSE_DIRECT_DCY_MODE

// Device Configuration for initialization
// change the define below to 1 to enable the option or 0 to disable it
// when initalizing the device this will be your inital configuration to it.

#define configENABLE_ALERT_RESPONSE          0  //0: ALERT interrupts are enabled. 
                                                //1: ALERT interrupts are masked, and the /TACH pin is always in a high impedance (open) state.
#define configENABLE_STANDBY_MODE            0  //0: Normal operation.
                                                //1: Standby mode. The device enters a low-power state.
#define configENABLE_PWM_STANDBY             0  //0: PWM output is enabled in standby mode.
                                                //1: PWM output is disabled (as defined by the PWM polarity bit) in standby mode.
#define configSELECT_ALERT_TACH_OUTPUT       0  //0: ALERT/TACH pin is an open drain ALERT output.
                                                //1: ALERT/TACH pin is a high-impedance TACH input.
#define configENABLE_T_CRIT_OVERRIDE         0  //0: locks the T_CRIT limit for the remote diode, POR setting is nominally 110°C
                                                //1: unlocks the T_CRIT limit and allows it to be reprogrammed multiple times.
#define configENABLE_RDTS_FAULT_QUEUE        0  //0: an ALERT will be generated if any Remote Diode conversion result is above the Remote High Set Point or below the Remote Low Setpoint.
                                                //1: an ALERT will be generated only if three consecutive Remote Diode conversions are above the Remote High Set Point or below the Remote Low Setpoint.
#define configUSE_ENHANCE_CONFIG             0  //0: Standard Configuration
                                                //1: Enhanced Configuration

// Device Enhanced Configuration
// if configUSE_ENHANCE_CONFIG is set to 1, the following configuration will be used, default is 0 for all configs.

#define configENABLE_SIGNED_TEMP_FILTER      0  //0: external signed temperature LSbs[4:3] will always read "0".
                                                //1: external signed temperature LSbs[4:3] (1/16 and 1/32 resolution) are enabled.
#define configENABLE_LOOKUP_TABLE_RES_EXT    0  //0: LUT temperature resolution 7-bits (LSb = 1°C).
                                                //1: enable 8-bit LUT temperature resolution (LSb extended to 0.5°C).
// This option only effective when PWM frequency set to 22.5kHz
#define configENABLE_PWM_HIGH_RES            0  //0: PWM resolution 6.25%. 
                                                //1: enable high resolution (0.39%).
#define configENABLE_UNSIGNED_H_T_CRIT_SP_FT 0  //0: enable signed format (11-bit is -128.000°C to 127.875°C or 8-bit is -128°C to 127°C).
                                                //1: enable unsigned format (11-bit is 0°C to 255.875°C or 8-bit is 0°C to 255°C).
#define configSET_PWM_SMOOTH_RAMP_RATE       0  //0: 0.023 s per step (5.45 seconds for 0 to 100% duty cycle transition with 0.39% resolution).
                                                //1: 0.046 s per step (10.9 seconds for 0 to 100% duty cycle transition with 0.39% resolution).
                                                //2: 0.091 s per step (21.9 seconds for 0 to 100% duty cycle transition with 0.39% resolution).
                                                //3: 0.182 s per step (43.8 seconds for 0 to 100% duty cycle transition with 0.39% resolution).
#define configENABLE_PWM_SMOOTH_RAMP_RATE    0  //0: PWM smoothing disabled.
                                                //1: enable ramp rate control.

// Look up table for Auto-Temp Mode

// Temperature (in °C) 
#define configLUT_TEMP_ENTRY_1               50  // 50°C
#define configLUT_TEMP_ENTRY_2               50  // 50°C
#define configLUT_TEMP_ENTRY_3               50  // 50°C
#define configLUT_TEMP_ENTRY_4               50  // 50°C
#define configLUT_TEMP_ENTRY_5               50  // 50°C
#define configLUT_TEMP_ENTRY_6               50  // 50°C
#define configLUT_TEMP_ENTRY_7               50  // 50°C
#define configLUT_TEMP_ENTRY_8               50  // 50°C
#define configLUT_TEMP_ENTRY_9               50  // 50°C
#define configLUT_TEMP_ENTRY_10              50  // 50°C
#define configLUT_TEMP_ENTRY_11              50  // 50°C
#define configLUT_TEMP_ENTRY_12              50  // 50°C

// PWM Duty Cycle
#define configLUT_PWM_ENTRY_1                0   // 0%
#define configLUT_PWM_ENTRY_2                0   // 0%
#define configLUT_PWM_ENTRY_3                0   // 0%
#define configLUT_PWM_ENTRY_4                0   // 0%
#define configLUT_PWM_ENTRY_5                0   // 0%
#define configLUT_PWM_ENTRY_6                0   // 0%
#define configLUT_PWM_ENTRY_7                0   // 0%
#define configLUT_PWM_ENTRY_8                0   // 0%
#define configLUT_PWM_ENTRY_9                0   // 0%
#define configLUT_PWM_ENTRY_10               0   // 0%
#define configLUT_PWM_ENTRY_11               0   // 0%
#define configLUT_PWM_ENTRY_12               0   // 0%

/* CTF2301 Exported Local Temperature Data */

// The local temperature resolution is 0.0625 °C. Temperature data is clamped and
// will not roll over when a temperature exceeds full-scale value.

typedef enum {
    LOCAL_TEMP_NEG_128      = 0x800,
    LOCAL_TEMP_NEG_100      = 0x9C0,
    LOCAL_TEMP_NEG_50       = 0xCE0,
    LOCAL_TEMP_0            = 0x000,
    LOCAL_TEMP_50           = 0x320,
    LOCAL_TEMP_100          = 0x640,
    LOCAL_TEMP_127          = 0x7FF
} LocalTemperature;

// When the extended resolution is enabled (Register Enhanced Configuration Bit
// STFBE is set), signed remote temperature resolution is 0.03125°C, otherwise, 
// the resolution is 0.125°C.

typedef enum {
    REMOTE_TEMP_S_NEG_128     = 0x1000,
    REMOTE_TEMP_S_NEG_100     = 0x1380,
    REMOTE_TEMP_S_NEG_50      = 0x19C0,
    REMOTE_TEMP_S_0           = 0x0000,
    REMOTE_TEMP_S_50          = 0x640,
    REMOTE_TEMP_S_100         = 0xC80,
    REMOTE_TEMP_S_127         = 0xFFF
} RemoteTemperatureSigned;

// If the extended resolution is enabled, the resolution of unsigned remote
// temperature is 0.03125°C, otherwise, the resolution is 0.125°C.

typedef enum {
    REMOTE_TEMP_US_0           = 0x0000,
    REMOTE_TEMP_US_50          = 0x640,
    REMOTE_TEMP_US_100         = 0xC80,
    REMOTE_TEMP_US_150         = 0x12C0,
    REMOTE_TEMP_US_200         = 0x1900,
    REMOTE_TEMP_US_255         = 0x1FFF
} RemoteTemperatureUnsigned;

/* CTF2301 Alert Status */

typedef enum {
    ALERT_STATUS_BUSY       = 0x80,
    ALERT_STATUS_LOCAL_HIGH = 0x40,
    ALERT_STATUS_REMOTE_HIGH = 0x10,
    ALERT_STATUS_REMOTE_LOW = 0x08,
    ALERT_STATUS_REMOTE_DIODE_FAULT = 0x04,
    ALERT_STATUS_REMOTE_T_CRIT_ALARM = 0x02,
    ALERT_STATUS_TACH_ALARM = 0x01
} AlertStatus;

/* CTF2301 Conversion Rate */

typedef enum {
    CONVERSION_RATE_0_05_HZ = 0x00,
    CONVERSION_RATE_0_1_HZ = 0x01,
    CONVERSION_RATE_0_204_HZ = 0x02,
    CONVERSION_RATE_0_406_HZ = 0x03,
    CONVERSION_RATE_1_HZ = 0x04,
    CONVERSION_RATE_1_626_HZ = 0x05,
    CONVERSION_RATE_3_257_HZ = 0x06,
    CONVERSION_RATE_6_494_HZ = 0x07,
    CONVERSION_RATE_9_303_HZ = 0x08  // All other values except defined are considered to be 9.303 Hz as well.
} ConversionRate;

/* CTF2301 Tachometer Mode */

typedef enum {
    TACH_MODE_00             = 0x00,    // Default , Traditional tach input monitor, false readings when under minimum detectable RPM. (Smart-TACH mode disabled)
    TACH_MODE_01             = 0x01,    // Traditional tach input monitor, FFFFh reading when under minimum detectable RPM. 
                                        // Smart-TACH mode enabled, PWM duty cycle not affected. Use with direct PWM drive of fan
                                        // power. TACH readings can cause an error event if TACH setpoint register is set to less than
                                        // FFFFh even though fan may be spinning properly.
    TACH_MODE_02             = 0x02,    // Most accurate readings, FFFFh reading when under minimum detectable RPM. Smart-TACH mode enabled, PWM duty cycle modified.
                                        // Use with direct PWM drive of fan power. This mode extends the TACH monitoring low RPM sensitivity.
    TACH_MODE_03             = 0x03     // Least effort on programmed PWM of fan, FFFFh reading when under minimum detectable RPM. Smart-TACH mode enabled. 
                                        // Use with direct PWM drive of fan power. This mode extends the TACH monitoring low RPM sensitivity the most.
} TachometerMode;


/* CTF2301 Register addresses. Set as 16-bit values */

typedef enum {
    LOCAL_TEMP = 0x0000,                    // Local temperature
    REMOTE_TEMP_MSB = 0x0001,                   // Remote temperature
    ALERT_STATUS = 0x0002,                  // Alert status
    CONFIG = 0x0003,                        // Configuration, 0x0009 is backup
    CONVERSION_RATE = 0x0004,               // Conversion rate, 0x000A is backup
    LOCAL_HIGH_SETPOINT_MSB = 0x0005,       // Local high setpoint MSB, 0x000B is backup
    LOCAL_HIGH_SETPOINT_LSB = 0x0006,       // Local high setpoint LSB
    REMOTE_HIGH_SETPOINT_MSB = 0x0007,      // Remote high setpoint MSB, 0x000D is backup
    REMOTE_LOW_SETPOINT_MSB = 0x0008,       // Remote low setpoint MSB, 0x000E is backup
    ONE_SHOT = 0x000F,                      // Write Only. Write command triggers one temperature conversion cycle.
    REMOTE_TEMP_LSB = 0x0010,               // Remote temperature LSB
    REMOTE_TEMP_OFFSET_MSB = 0x0011,        // Remote temperature offset MSB
    REMOTE_TEMP_OFFSET_LSB = 0x0012,        // Remote temperature offset LSB
    REMOTE_HIGH_SETPOINT_LSB = 0x0013,      // Remote high setpoint LSB
    REMOTE_LOW_SETPOINT_LSB = 0x0014,       // Remote low setpoint LSB
    LOCAL_TEMP_LSB = 0x0015,                // Local temperature LSB
    ALERT_MASK = 0x0016,                    // Alert mask
    REMOTE_T_CRIT_SETPOINT = 0x0019,        // Remote T_CRIT setpoint
    REMOTE_T_CRIT_HYST = 0x0021,            // Remote T_CRIT hysteresis
    REMOTE_DIODE_BETA_COMP = 0x0030,        // Remote diode beta compensation
    REMOTE_TEMP_UNSIGNED_MSB = 0x0031,      // Remote temperature unsigned MSB
    REMOTE_TEMP_UNSIGNED_LSB = 0x0032,      // Remote temperature unsigned LSB
    POR_STATUS = 0x0033,                    // Power-on reset status
    NC_FACTORY = 0x0034,                    // Not calibrated factory trim
    SMBUS_TIMEOUT = 0x0037,                 // SMBus timeout
    ENHANCED_CONFIG = 0x0045,               // Enhanced configuration
    TACH_COUNT_LSB = 0x0046,                // Tachometer count LSB
    TACH_COUNT_MSB = 0x0047,                // Tachometer count MSB
    TACH_LIMIT_LSB = 0x0048,                // Tachometer limit LSB
    TACH_LIMIT_MSB = 0x0049,                // Tachometer limit MSB
    PWM_TACH_CONFIG = 0x004A,               // PWM and tachometer configuration
    FAN_SPIN_UP_CONFIG = 0x004B,            // Fan spin-up configuration
    PWM_VALUE = 0x004C,                     // PWM value
    PWM_FREQ = 0x004D,                      // PWM frequency
    LOOKUP_TABLE_OFFSET = 0x004E,           // Lookup table offset
    LOOKUP_TABLE_HYST = 0x004F,             // Lookup table hysteresis
    LOOKUP_TABLE_TEMP_1 = 0x0050,                // Lookup Table of up to 12 PWM (3F) and Temp Pairs in 8-bit Registers (7F)
    LOOKUP_TABLE_PWM_1 = 0x0051,
    LOOKUP_TABLE_TEMP_2 = 0x0052,
    LOOKUP_TABLE_PWM_2 = 0x0053,
    LOOKUP_TABLE_TEMP_3 = 0x0054,
    LOOKUP_TABLE_PWM_3 = 0x0055,
    LOOKUP_TABLE_TEMP_4 = 0x0056,
    LOOKUP_TABLE_PWM_4= 0x0057,
    LOOKUP_TABLE_TEMP_5 = 0x0058,
    LOOKUP_TABLE_PWM_5 = 0x0059,
    LOOKUP_TABLE_TEMP_6 = 0x005A,
    LOOKUP_TABLE_PWM_6 = 0x005B,
    LOOKUP_TABLE_TEMP_7 = 0x005C,
    LOOKUP_TABLE_PWM_7 = 0x005D,
    LOOKUP_TABLE_TEMP_8 = 0x005E,
    LOOKUP_TABLE_PWM_8 = 0x005F,
    LOOKUP_TABLE_TEMP_9 = 0x0060,
    LOOKUP_TABLE_PWM_9 = 0x0061,
    LOOKUP_TABLE_TEMP_10 = 0x0062,
    LOOKUP_TABLE_PWM_10 = 0x0063,
    LOOKUP_TABLE_TEMP_11 = 0x0064,
    LOOKUP_TABLE_PWM_11 = 0x0065,
    LOOKUP_TABLE_TEMP_12 = 0x0066,
    LOOKUP_TABLE_PWM_12 = 0x0067,
    REMOTE_DIODE_TEMP_FILTER = 0x00BF,      // Remote diode temperature filter
    STEP_DIE_REV_ID = 0x00FE,               // Step and die revision ID, Fixed value 0x01
    MANUFACTURER_ID = 0x00FF,               // Manufacturer ID, Fixed value 0x59
} CTF2301_Register;

// Register Handling Function Prototypes

//
// Fan PWM and TACH Configuration
// PWM Programming Enable, Default is Enabled
// Return: CTF2301_OK if enable is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_ENABLE_PWM_PROGRAMMING();
uint32_t __CTF2301_DISABLE_PWM_PROGRAMMING();

// PWM Output Polarity, Default is RISING edge
// Param:
// 0: 0V for fan OFF and open for fan ON
// 1: open for fan OFF and 0V for fan ON
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_PWM_POLARITY(uint8_t param);

// PWM Master Clock Select， Default is 360kHz
// 0: 360kHz
// 1: 1.4kHz
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_PWM_MASTER_CLOCK(uint8_t param);

// Tachometer Mode Select
// Note: If the PWM Master Clock is 360 kHz, mode 00 is used regardless of the setting of these two bits.
// Return: CTF2301_OK if selection is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_TACH_MODE(TachometerMode mode);
//

//
// Fan Spin-up Configuration
// Fast Tachometer Spin-up, Default is 0x01
// Param:
// 0x00: the fan spin-up uses the duty cycle (bits 4:3) and spin-up time (bits 2:0).
// 0x01: sets the PWM output to 100% until the spin-up times out (bits 2:0) or the minimum desired RPM
// has been reached (per the Tachometer Setpoint setting) using the tachometer input, whichever
// happens first. This bit overrides the PWM Spin-Up Duty Cycle register (bits 4:3) PWM output is
// always 100%. Register x03, bit 2 = 1 for Tachometer mode.
// If PWM Spin-Up Time (bits 2:0) = 000, the Spin-Up cycle is bypassed, regardless of the state of this bit.
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_FAST_TACHOMETER_SPIN_UP(uint8_t param); 

// PWM Spin-Up Duty Cycle, Default is 0x01
// Param:
// 0x00: 0% Spin-Up cycle bypassed (no Spin-Up), unless Fast Tachometer Terminated Spin-Up (bit 5) is set.
// 0x01: 50%
// 0x02: 75% 81% Depends on PWM Frequency.
// 0x03: 100%
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_PWM_SPIN_UP_DUTY_CYCLE(uint8_t param);

// PWM Spin-Up Time Interval, Default is 0x01
// Param:
// 0x00: By-passed, no spin-up.
// 0x01: 0.05 seconds
// 0x02: 0.1 seconds
// 0x03: 0.2 seconds
// 0x04: 0.4 seconds
// 0x05: 0.8 seconds
// 0x06: 1.6 seconds
// 0x07: 3.2 seconds
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_PWM_SPIN_UP_TIME_INTERVAL(uint8_t param);
//

// Read or Write PWM Duty Cycle for direct fan speed control, Default is 0x00 (means off)
uint32_t __CTF2301_SET_PWM_VALUE(uint8_t param);
uint32_t __CTF2301_GET_PWM_VALUE(uint8_t *param);

// Set PWM Output Frequency. Default is 0x17 (7.82KHZ @ 360KHZ Master Clock, 30HZ @ 1.4KHZ Master Clock)
// Param:
// Only PWMF[4:0] is used. This sets the n value, then the frequency is calculated base on following formula:
// f = PWM_CLOCK / (2 * n), where PWM_CLOCK can be set by __CTF2301_SET_PWM_MASTER_CLOCK.
uint32_t __CTF2301_SET_PWM_OUTPUT_FREQUENCY(uint8_t param);

// Set Lookup Table Temp Offset. Default is 0x00
// TODO

// Set Lookup Table Hysteresis. Default is 0x04
// TODO

// Setup Lookup Table. Default is 0x7F
uint32_t __CTF2301_SET_LOOKUP_TABLE();

// Set Remote Diode Beta Compensation. Default is 0x82
// TODO

// Set Remote Diode Temperature Filter and Comparator Mode, Default is 0x00
// TODO

// Set SMBus Timeout. Default is 0x00
// TODO

// Set Alarm Mask. Default is 0x00
// TODO

// Set Local Temperature limit
// TODO

// Set Remote Temperature limit
// TODO

// Set Tachometer limit
// TODO

// Tach measurement
// Param: tach - return Tachometer reading
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_readTach(uint16_t *tach);

// Basic R/W
// Read a register from the CTF2301
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_readRegister(CTF2301_Register address, uint8_t *buffer);

// Write a register to the CTF2301
// Return: CTF2301_OK if writing is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_writeRegister(CTF2301_Register address, uint8_t data);

// Exported Function Prototypes
// Initialize the CTF2301, you have to select the options in the defines above
// Return: CTF2301_OK if the initialization is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_init();

// Check Power On Reset Status
// Return: CTF2301_OK if POR is ready, CTF2301_ERROR_NOT_READY otherwise
uint32_t CTF2301_checkPOR();

// Read Manufacturer ID.
// Param: id - return Manufacturer ID
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_readManufacturerID(uint8_t *id);

// Read Step and Die Revision ID.
// Param: id - return Step and Die Revision ID
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_readStepDieRevID(uint8_t *id);

// Set Fan Speed (in PWM Duty Cycle)
// Param: fanMaxRPM - Maximum RPM of the fan, setRPM - Desired RPM
// Note: The maximum RPM of the fan varies from fan to fan specs and may not be accurate, the actual RPM may vary. 
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_setFanSpeed(uint16_t fanMaxRPM, uint16_t setRPM);

// Get Fan Speed (in RPM)
// Param: rpm - return RPM
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_getFanSpeed(uint16_t *rpm);

// Get Rounded Remote Temperature
// Param: temp - return Remote Temperature (Rounded)
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_getRdRemoteTemp(uint16_t *temp);

#ifdef __cplusplus
}
#endif

#endif /* INC_PCM51XX_H_ */
