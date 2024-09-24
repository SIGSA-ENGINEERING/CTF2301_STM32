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
 */

#include "CTF2301.h"
#include <stdio.h>

//PV
uint8_t ctf2301_i2cAddr = configDEVICE_CTF2301_I2C_ADDR;

// PWM Programming Enable, Default is Enabled
// Return: CTF2301_OK if enable is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_ENABLE_PWM_PROGRAMMING(){
    uint32_t ret = CTF2301_OK;
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(PWM_TACH_CONFIG, &regData) == CTF2301_OK){
        regData |= 0x20;
        if (__CTF2301_writeRegister(PWM_TACH_CONFIG, regData) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

uint32_t __CTF2301_DISABLE_PWM_PROGRAMMING(){
    uint32_t ret = CTF2301_OK;
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(PWM_TACH_CONFIG, &regData) == CTF2301_OK){
        regData &= ~0x20;
        if (__CTF2301_writeRegister(PWM_TACH_CONFIG, regData) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

// PWM Output Polarity, Default is RISING edge
// Param:
// 0: 0V for fan OFF and open for fan ON
// 1: open for fan OFF and 0V for fan ON
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_PWM_POLARITY(uint8_t param){
    uint32_t ret = CTF2301_OK;
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(PWM_TACH_CONFIG, &regData) == CTF2301_OK){
        if (param == 0){
            regData &= ~0x10;
        } else {
            regData |= 0x10;
        }
        if (__CTF2301_writeRegister(PWM_TACH_CONFIG, regData) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

// PWM Master Clock Select， Default is 360kHz
// 0: 360kHz
// 1: 1.4kHz
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_PWM_MASTER_CLOCK(uint8_t param){
    uint32_t ret = CTF2301_OK;
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(PWM_TACH_CONFIG, &regData) == CTF2301_OK){
        if (param == 0){
            regData &= ~0x08;
        } else {
            regData |= 0x08;
        }
        if (__CTF2301_writeRegister(PWM_TACH_CONFIG, regData) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

// Tachometer Mode Select
// Note: If the PWM Master Clock is 360 kHz, mode 00 is used regardless of the setting of these two bits.
// Return: CTF2301_OK if selection is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_TACH_MODE(TachometerMode mode){
    uint32_t ret = CTF2301_OK;
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(PWM_TACH_CONFIG, &regData) == CTF2301_OK){
        regData &= ~0x03;
        regData |= mode;
        if (__CTF2301_writeRegister(PWM_TACH_CONFIG, regData) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

// Fast Tachometer Spin-up, Default is 0x01
// Param:
// 0x00: the fan spin-up uses the duty cycle (bits 4:3) and spin-up time (bits 2:0).
// 0x01: sets the PWM output to 100% until the spin-up times out (bits 2:0) or the minimum desired RPM
// has been reached (per the Tachometer Setpoint setting) using the tachometer input, whichever
// happens first. This bit overrides the PWM Spin-Up Duty Cycle register (bits 4:3) PWM output is
// always 100%. Register x03, bit 2 = 1 for Tachometer mode.
// If PWM Spin-Up Time (bits 2:0) = 000, the Spin-Up cycle is bypassed, regardless of the state of this bit.
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_FAST_TACHOMETER_SPIN_UP(uint8_t param){
    uint32_t ret = CTF2301_OK;
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(FAN_SPIN_UP_CONFIG, &regData) == CTF2301_OK){
        if (param == 0){
            regData &= ~0x20;
        } else {
            regData |= 0x20;
        }
        if (__CTF2301_writeRegister(FAN_SPIN_UP_CONFIG, regData) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

// PWM Spin-Up Duty Cycle, Default is 0x01
// Param:
// 0x00: 0% Spin-Up cycle bypassed (no Spin-Up), unless Fast Tachometer Terminated Spin-Up (bit 5) is set.
// 0x01: 50%
// 0x02: 75% 81% Depends on PWM Frequency.
// 0x03: 100%
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_SET_PWM_SPIN_UP_DUTY_CYCLE(uint8_t param){
    uint32_t ret = CTF2301_OK;
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(FAN_SPIN_UP_CONFIG, &regData) == CTF2301_OK){
        regData &= ~0x18;
        regData |= (param << 3);
        if (__CTF2301_writeRegister(FAN_SPIN_UP_CONFIG, regData) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

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
uint32_t __CTF2301_SET_PWM_SPIN_UP_TIME_INTERVAL(uint8_t param){
    uint32_t ret = CTF2301_OK;
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(FAN_SPIN_UP_CONFIG, &regData) == CTF2301_OK){
        regData &= ~0x07;
        regData |= param;
        if (__CTF2301_writeRegister(FAN_SPIN_UP_CONFIG, regData) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

// Read or Write PWM Duty Cycle for direct fan speed control, Default is 0x00 (means off)
uint32_t __CTF2301_SET_PWM_VALUE(uint8_t param){
    uint32_t ret = CTF2301_OK;
    // This is only available when PWM Programming is enabled
    // Read PWPGM bit
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(PWM_TACH_CONFIG, &regData) == CTF2301_OK){
        if ((regData & 0x20) == 0){
            ret = CTF2301_ERROR_NOT_READY;
        }
    } else {
        if (__CTF2301_writeRegister(PWM_VALUE, param) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    }

    return ret;
}

uint32_t __CTF2301_GET_PWM_VALUE(uint8_t *param){
    uint32_t ret = CTF2301_OK;
    if (__CTF2301_readRegister(PWM_VALUE, param) == CTF2301_OK){
    	ret = CTF2301_ERROR;
    }
    return ret;
}

// Set PWM Output Frequency. Default is 0x17 (7.82KHZ @ 360KHZ Master Clock, 30HZ @ 1.4KHZ Master Clock)
// Param:
// Only PWMF[4:0] is used. This sets the n value, then the frequency is calculated base on following formula:
// f = PWM_CLOCK / (2 * n), where PWM_CLOCK can be set by __CTF2301_SET_PWM_MASTER_CLOCK.
uint32_t __CTF2301_SET_PWM_OUTPUT_FREQUENCY(uint8_t param){
    uint32_t ret = CTF2301_OK;
    uint8_t regData = 0x00;
    if (__CTF2301_readRegister(PWM_FREQ, &regData) == CTF2301_OK){
        regData &= ~0x1F;
        regData |= param;
        if (__CTF2301_writeRegister(PWM_FREQ, regData) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

// Setup Lookup Table. Default is 0x7F
uint32_t __CTF2301_SET_LOOKUP_TABLE(){
    uint32_t ret = CTF2301_OK;
    uint8_t regAddrTemp[12] = {LOOKUP_TABLE_TEMP_1, 
                           LOOKUP_TABLE_TEMP_2, 
                           LOOKUP_TABLE_TEMP_3, 
                           LOOKUP_TABLE_TEMP_4, 
                           LOOKUP_TABLE_TEMP_5, 
                           LOOKUP_TABLE_TEMP_6, 
                           LOOKUP_TABLE_TEMP_7, 
                           LOOKUP_TABLE_TEMP_8, 
                           LOOKUP_TABLE_TEMP_9, 
                           LOOKUP_TABLE_TEMP_10, 
                           LOOKUP_TABLE_TEMP_11, 
                           LOOKUP_TABLE_TEMP_12};
    uint8_t regAddrPWM[12] = {LOOKUP_TABLE_PWM_1,
                            LOOKUP_TABLE_PWM_2,
                            LOOKUP_TABLE_PWM_3,
                            LOOKUP_TABLE_PWM_4,
                            LOOKUP_TABLE_PWM_5,
                            LOOKUP_TABLE_PWM_6,
                            LOOKUP_TABLE_PWM_7,
                            LOOKUP_TABLE_PWM_8,
                            LOOKUP_TABLE_PWM_9,
                            LOOKUP_TABLE_PWM_10,
                            LOOKUP_TABLE_PWM_11,
                            LOOKUP_TABLE_PWM_12};
    uint8_t regDataTemp[12] = {configLUT_TEMP_ENTRY_1,
                            configLUT_TEMP_ENTRY_2,
                            configLUT_TEMP_ENTRY_3,
                            configLUT_TEMP_ENTRY_4,
                            configLUT_TEMP_ENTRY_5,
                            configLUT_TEMP_ENTRY_6,
                            configLUT_TEMP_ENTRY_7,
                            configLUT_TEMP_ENTRY_8,
                            configLUT_TEMP_ENTRY_9,
                            configLUT_TEMP_ENTRY_10,
                            configLUT_TEMP_ENTRY_11,
                            configLUT_TEMP_ENTRY_12};
    uint8_t regDataPWM[12] = {configLUT_PWM_ENTRY_1,
                            configLUT_PWM_ENTRY_2,
                            configLUT_PWM_ENTRY_3,
                            configLUT_PWM_ENTRY_4,
                            configLUT_PWM_ENTRY_5,
                            configLUT_PWM_ENTRY_6,
                            configLUT_PWM_ENTRY_7,
                            configLUT_PWM_ENTRY_8,
                            configLUT_PWM_ENTRY_9,
                            configLUT_PWM_ENTRY_10,
                            configLUT_PWM_ENTRY_11,
                            configLUT_PWM_ENTRY_12};
    for (int i = 0; i < 12; i++){
        if (__CTF2301_writeRegister(regAddrTemp[i], regDataTemp[i]) != CTF2301_OK){
            ret = CTF2301_ERROR;
            break;
        }
        if (__CTF2301_writeRegister(regAddrPWM[i], regDataPWM[i]) != CTF2301_OK){
            ret = CTF2301_ERROR;
            break;
        }
    }

    return ret;
}

// Tach measurement
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_readTach(uint16_t *tach){
    //TODO
}

// Basic R/W
// Read a register from the CTF2301
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_readRegister(CTF2301_Register address, uint8_t *buffer){
    uint32_t ret = CTF2301_OK;
    if (HAL_I2C_Mem_Read(&CTF2301_I2C_HANDLE, ctf2301_i2cAddr << 1, address, I2C_MEMADD_SIZE_8BIT, buffer, 1, HAL_MAX_DELAY) != HAL_OK){
        ret = CTF2301_ERROR;
    }

    return ret;
}

// Write a register to the CTF2301
// Return: CTF2301_OK if writing is successful, CTF2301_ERROR otherwise
uint32_t __CTF2301_writeRegister(CTF2301_Register address, uint8_t data){
    uint32_t ret = CTF2301_OK;
    if (HAL_I2C_Mem_Write(&CTF2301_I2C_HANDLE, ctf2301_i2cAddr << 1, address, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY) != HAL_OK){
        ret = CTF2301_ERROR;
    }

    return ret;

}

// Initialize the CTF2301, you have to select the options in the defines above
// Return: CTF2301_OK if the initialization is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_init(){
    uint32_t ret = CTF2301_OK;
    uint8_t id_data;
    uint8_t config_data = 0x00;
    uint8_t enhanced_config_data = 0x00;

    // Check if Not Ready Bit is clear in POR
    if (CTF2301_checkPOR() != CTF2301_OK){
        ret = CTF2301_ERROR_NOT_READY;
        return ret;
    }

    // Check the device ID
    if (CTF2301_readManufacturerID(&id_data) == CTF2301_OK){
        if(id_data != CTF2301_MANUFACTURER_ID){
            ret = CTF2301_ERROR_ID;
            return ret;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
        return ret;
    }

    // Configure PWM TACH
    // PWM TACH Power on Default is 0x20, you can change the values here so it can setup once and for all during init
    // But I will comment out here since default usually works fine.
    /*
    // Set Tachometer Mode
    __CTF2301_SET_TACH_MODE(TACH_MODE_01);
    // Set PWM output polarity
    __CTF2301_SET_PWM_POLARITY(1);
    // Set PWM Master Clock
    __CTF2301_SET_PWM_MASTER_CLOCK(1);
    */

    // Configure Fan Spin-up
    // Default value is 0x3F, you can change according to your needs. But I will comment out here since default usually works fine.
    /*
    // Set Fast Tachometer Spin-up
    __CTF2301_SET_FAST_TACHOMETER_SPIN_UP(0x00);
    // Set PWM Spin-Up Duty Cycle
    __CTF2301_SET_PWM_SPIN_UP_DUTY_CYCLE(0x00);
    // Set PWM Spin-Up Time Interval
    __CTF2301_SET_PWM_SPIN_UP_TIME_INTERVAL(0x00);
    */

    // Configure PWM Frequency
    // Default value is 0x17, you can change according to your needs. Refer to the header file and datasheet for how to set the frequency.
    /*
    // Set PWM Output Frequency
    __CTF2301_SET_PWM_OUTPUT_FREQUENCY(0x17);
    */

    // DEVICE OPTION 1 (You can only choose one of the two options)
    // Configure Look-up Table LUT (This determines the Auto-Temp Mode Temp to Fan Speed Ratio)
    // The look-up table has a set of preset data in it, you can change it according to your application needs.
    
    #ifndef configUSE_DIRECT_DCY_MODE

    // Setup Look-up Table
    __CTF2301_SET_LOOKUP_TABLE();
    // Disable Program Mode to enable Auto Mode
    __CTF2301_DISABLE_PWM_PROGRAMMING();

    #endif

    // DEVICE OPTION 2
    // Configure PWM Value (for manual direct fan speed control)
    
    #ifdef configUSE_DIRECT_DCY_MODE
        __CTF2301_SET_PWM_VALUE(0x00); // Default is 0x00 (means off)
    #endif

    // Set the configuration
    #if (configENABLE_ALERT_RESPONSE == 1)
        config_data |= 0x80;
    #endif
    #if (configENABLE_STANDBY_MODE == 1)
        config_data |= 0x40;
    #endif 
    #if (configENABLE_PWM_STANDBY == 1)
        config_data |= 0x20;
    #endif
    #if (configSELECT_ALERT_TACH_OUTPUT == 1)
        config_data |= 0x10;
    #endif
    #if (configENABLE_T_CRIT_OVERRIDE == 1)
        config_data |= 0x08;
    #endif
    #if (configENABLE_RDTS_FAULT_QUEUE == 1)
        config_data |= 0x04;
    #endif
    
    if (config_data != 0x00){ // not default
        if (__CTF2301_writeRegister(CONFIG, config_data) != CTF2301_OK){
            ret = CTF2301_ERROR;
        }
    }

    // Set the enhanced configuration
    #if (configENABLE_SIGNED_TEMP_FILTER == 1)
        enhanced_config_data |= 0x40;
    #endif
    #if (configENABLE_LOOKUP_TABLE_RES_EXT == 1)
        enhanced_config_data |= 0x20;
    #endif
    #if (configENABLE_PWM_HIGH_RES == 1)
        enhanced_config_data |= 0x10;
    #endif
    #if (configENABLE_UNSIGNED_H_T_CRIT_SP_FT == 1)
        enhanced_config_data |= 0x08;
    #endif
    #if (configSET_PWM_SMOOTH_RAMP_RATE == 1)
        enhanced_config_data |= 0x02;
    #endif
    #if (configSET_PWM_SMOOTH_RAMP_RATE == 2)
        enhanced_config_data |= 0x04;
    #endif
    #if (configSET_PWM_SMOOTH_RAMP_RATE == 3)
        enhanced_config_data |= 0x06;
    #endif
    #if (configENABLE_PWM_SMOOTH_RAMP_RATE == 1)
        enhanced_config_data |= 0x01;
    #endif

    #if (configUSE_ENHANCE_CONFIG == 1)
        if (enhanced_config_data != 0x00){ // not default
            if (__CTF2301_writeRegister(ENHANCED_CONFIG, enhanced_config_data) != CTF2301_OK){
                ret = CTF2301_ERROR;
            }
        }
    #endif

    return ret;
}

// Check Power On Reset Status
// Return: CTF2301_OK if POR is ready, CTF2301_ERROR_NOT_READY otherwise
uint32_t CTF2301_checkPOR(){
    uint32_t ret = CTF2301_OK;
    uint8_t por_data = 0x00;
    if (__CTF2301_readRegister(POR_STATUS, &por_data) == CTF2301_OK){
        if (por_data != 0x00){ // Power On Not Ready
            ret = CTF2301_ERROR_NOT_READY;
        }
    } else {
        ret = CTF2301_ERROR_COMM;
    }
    return ret;
}

// Read Manufacturer ID.
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_readManufacturerID(uint8_t *id){
    uint32_t ret = CTF2301_OK;
    if (__CTF2301_readRegister(MANUFACTURER_ID, id) != CTF2301_OK){
        ret = CTF2301_ERROR;
    }
    return ret;
}

// Read Step and Die Revision ID.
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_readStepDieRevID(uint8_t *id){
    uint32_t ret = CTF2301_OK;
    if (__CTF2301_readRegister(STEP_DIE_REV_ID, id) != CTF2301_OK){
        ret = CTF2301_ERROR;
    }
    return ret;
}


// Set Fan Speed (in PWM Duty Cycle)
// Param: fanMaxRPM - Maximum RPM of the fan, setRPM - Desired RPM
// Note: The maximum RPM of the fan varies from fan to fan specs and may not be accurate, the actual RPM may vary. 
// Return: CTF2301_OK if setting is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_setFanSpeed(uint16_t fanMaxRPM, uint16_t setRPM){
    //TODO
}

// Get Fan Speed (in RPM)
// Param: rpm - return RPM
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_getFanSpeed(uint16_t *rpm){
    //TODO
}

// Get Rounded Remote Temperature
// Param: temp - return Remote Temperature (Rounded)
// Return: CTF2301_OK if reading is successful, CTF2301_ERROR otherwise
uint32_t CTF2301_getRdRemoteTemp(uint16_t *temp){
    uint32_t ret = CTF2301_OK;
    if (__CTF2301_readRegister(REMOTE_TEMP_MSB, temp) != CTF2301_OK){
        ret = CTF2301_ERROR;
    }
    return ret;
}
