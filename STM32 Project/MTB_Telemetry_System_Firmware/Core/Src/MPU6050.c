#include "MPU6050.h"

uint8_t MPU6050_initialize(void)
{
	uint8_t check;

	// Check the device id
	HAL_I2C_Mem_Read(&handler_MPU6050, MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1, 100);

	if (check == 104)
	{
		// Set power register to 0
		uint8_t command = 0;
		HAL_I2C_Mem_Write(&handler_MPU6050, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &command, 1, 100);

		// Set data rate of 1kHz
		command = 0x07;
		HAL_I2C_Mem_Write(&handler_MPU6050, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &command, 1, 100);

		// Accelerometer configuration
		command = 0;
		HAL_I2C_Mem_Write(&handler_MPU6050, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &command, 1, 100);

		// Gyroscope config
		command = 0;
		HAL_I2C_Mem_Write(&handler_MPU6050, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &command, 1, 100);

		return 0;
	}
	else
	{
		return 1; // error
	}
}


int16_t MPU6050_gyro_read(uint8_t axis)
{
	uint8_t raw_data[6];

	// Read raw data from register
	HAL_I2C_Mem_Read(&handler_MPU6050, MPU6050_ADDR, GYRO_XOUT_H_REG, 1, raw_data, 6, 100);

	if (axis == Xaxis)
	{
		return ((int16_t)(raw_data[0] << 8 | raw_data[1]) / GYRO_FS);
	}
	else if (axis == Yaxis)
	{
		return ((int16_t)(raw_data[2] << 8 | raw_data[3]) / GYRO_FS);
	}
	else if(axis == Zaxis)
	{
		return ((int16_t)(raw_data[4] << 8 | raw_data[5]) / GYRO_FS);
	}
	else
	{
		return 999; // wrong input
	}
}


int16_t MPU6050_accel_read(uint8_t axis)
{
	uint8_t raw_data[6];

	// Read raw data from register
	HAL_I2C_Mem_Read(&handler_MPU6050, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, raw_data, 6, 100);

	if (axis == Xaxis)
	{
		return ((int16_t)(raw_data[0] << 8 | raw_data[1]) / ACCEL_FS);
	}
	else if (axis == Yaxis)
	{
		return ((int16_t)(raw_data[2] << 8 | raw_data[3]) / ACCEL_FS);
	}
	else if(axis == Zaxis)
	{
		return ((int16_t)(raw_data[4] << 8 | raw_data[5]) / ACCEL_FS_Z);
	}
	else
	{
		return 999; // wrong input
	}
}
