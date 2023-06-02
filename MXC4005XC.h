#include <stdint.h>
#include <Wire.h>

#define MXC4005XC_ADDRESS           0x15 // TODO: make it work with 0x10 to 0x17

#define MXC4005XC_REG_DATA          0x03
#define MXC4005XC_REG_CTRL          0x0D
#define MXC4005XC_REG_DEVICE_ID     0x0E

#define MXC4005XC_REG_INT_SRC0      0x00
#define MXC4005XC_REG_INT_SRC1      0x01
#define MXC4005XC_REG_INT_CLR0      0x00
#define MXC4005XC_REG_INT_CLR1      0x01
#define MXC4005XC_REG_INT_MASK0     0x0A
#define MXC4005XC_REG_INT_MASK1     0x0B
#define MXC4005XC_REG_DETECTION     0x0C

#define MXC4005XC_CMD_8G_POWER_ON   0x40
#define MXC4005XC_CMD_4G_POWER_ON   0x20
#define MXC4005XC_CMD_2G_POWER_ON   0x00
#define MXC4005XC_CMD_POWER_DOWN    0x01


#define MXC4005XC_DEVICE_ID_1       0x02
#define MXC4005XC_DEVICE_ID_2       0x03

#define MXC4005XC_2G_SENSITIVITY    1024
#define MXC4005XC_4G_SENSITIVITY    512
#define MXC4005XC_8G_SENSITIVITY    256
#define MXC4005XC_T_ZERO            25
#define MXC4005XC_T_SENSITIVITY     0.586

uint8_t MXC4005XC_Enable(uint8_t sda, uint8_t scl);

// Get sensor data:
// data[0..2] is the accelerometer vector (x,y,z), unit is g
// data[3]    is the temperature, unit is degree Celsius
void MXC4005XC_GetData(float *data);

