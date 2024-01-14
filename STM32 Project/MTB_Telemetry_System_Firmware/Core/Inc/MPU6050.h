#include "stm32f4xx_hal.h"

#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define GYRO_CONFIG_REG 0x1B
#define GYRO_XOUT_H_REG 0x43
#define MPU6050_ADDR 0xD0

#define GYRO_FS 131
#define ACCEL_FS 16384
#define ACCEL_FS_Z 14418


// I2C handler
I2C_HandleTypeDef hi2c2; // Rear sensor
#define handler_MPU6050 hi2c2


// Axis definition
#define Xaxis 0
#define Yaxis 1
#define Zaxis 2


// Initialize the MPU6050
uint8_t MPU6050_initialize(void);

// Get raw gyroscope reading
int16_t MPU6050_gyro_read(uint8_t axis);

// Get raw accelerometer reading
int16_t MPU6050_accel_read(uint8_t axis);
