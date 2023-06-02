#include "MXC4005XC.h"

#define INITIAL_TRX 1 // neutral value


//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(...)     Serial.print(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif


////////////////////////////////////////////////////////////////////////////////
// Arduino abstraction layer
////////////////////////////////////////////////////////////////////////////////

uint8_t I2C_Write_Reg(uint8_t i2c_add, uint8_t reg_add, uint8_t cmd)
{
	// i2c_add is the 7-bit i2c address of the sensor
	// reg_add is the register address to wtite
	// cmd is the value that need to be written to the register
	// I2C operating successfully, return 1, otherwise return 0;
    uint8_t trx = INITIAL_TRX; // transceived number of bytes

    Wire.beginTransmission(i2c_add);
	trx *= Wire.write(reg_add);
	trx *= Wire.write(cmd);
    Wire.endTransmission();

	return trx;
}

inline uint8_t I2C_MultiRead_Reg(uint8_t i2c_add, uint8_t reg_add, uint8_t num, uint8_t *data)
{
	// i2c_add is the 7-bit i2c address of the sensor
	// reg_add is the first register address to read
	// num is the number of the register to read
	// data is the address of the buffer
	// I2C operating successfully, return 1, otherwise return 0;
    uint8_t trx = INITIAL_TRX;

    Wire.beginTransmission(i2c_add);
    trx *= Wire.write(reg_add);
    Wire.endTransmission(false); //endTransmission but keep connection active

    // Ask for bytes, once done, bus is released by default
    trx *= Wire.requestFrom(i2c_add, num);

    // Wait until we get the number of bytes we expect
    while (Wire.available() < num);

    for (uint8_t i = 0; i < num; i++)
        data[i] = Wire.read();

    return trx; // transceived number of bytes
}

inline uint8_t I2C_Read_Reg(uint8_t i2c_add, uint8_t reg_add, uint8_t *data)
{
    uint8_t trx = Wire.requestFrom(i2c_add, 1, reg_add, 1, true);
    *data = Wire.read();
    return trx; // transceived number of bytes
}

inline void I2C_init(uint8_t sda, uint8_t scl)
{
    Wire.setSDA(sda);
    Wire.setSCL(scl);
    Wire.begin();
}


//////////////////////////////////////////////////////////////////////////////////
// enable the sensor - from power down mode to normal mode
//////////////////////////////////////////////////////////////////////////////////
uint8_t MXC4005XC_Enable(uint8_t sda, uint8_t scl)
{
    uint8_t trx = INITIAL_TRX; // transceived number of bytes (0 == error)

    // init i2c pins
    I2C_init(sda, scl);

    // check sensor id
    uint8_t reg_val = 0;
    trx *= I2C_Read_Reg(MXC4005XC_ADDRESS, MXC4005XC_REG_DEVICE_ID, &reg_val);
    if (trx == 0) {
        DEBUG_PRINT("E\n");
    }

    if((reg_val != MXC4005XC_DEVICE_ID_1)&&(reg_val != MXC4005XC_DEVICE_ID_2)) {
        DEBUG_PRINT(reg_val);
        DEBUG_PRINT('\n'); // save a few bytes vs println!
        return 0;
    }

    // Power On the sensor and set the 2g scale
    trx *= I2C_Write_Reg(MXC4005XC_ADDRESS, MXC4005XC_REG_CTRL, MXC4005XC_CMD_2G_POWER_ON);

    return trx;
}

/////////////////////////////////////////////////////////////////////////////////
// read sensor data
/////////////////////////////////////////////////////////////////////////////////
void MXC4005XC_GetData(float *data)
{
    uint8_t data_reg[7] = {0};

    // read the register data (and check the number of transmitted bytes)
    if (!I2C_MultiRead_Reg(MXC4005XC_ADDRESS, MXC4005XC_REG_DATA, 7, data_reg)) {
        DEBUG_PRINT("E\n");
    }

    for (uint8_t i = 0; i < 3; i++) {
        data[i] = (float)( (int16_t)(data_reg[i*2]<<8 | data_reg[i*2 + 1]) >> 4 );

        // convert acceleration to g
        data[i] /= MXC4005XC_2G_SENSITIVITY;
    }

    // convert to unit is degree Celsius
    data[3] = (float)data_reg[6] * MXC4005XC_T_SENSITIVITY + MXC4005XC_T_ZERO;
}

