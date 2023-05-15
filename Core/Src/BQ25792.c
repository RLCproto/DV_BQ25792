#include "BQ25792.h"
#include "stm32l4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef BQ25792_Read(uint8_t reg, uint8_t *pData, uint8_t len)
{
	HAL_StatusTypeDef errCode = HAL_OK;
	if(len > 2 || pData == NULL)
		return HAL_ERROR;
	errCode = HAL_I2C_Master_Transmit(&hi2c1, BQ25792_ADRESS, &reg, 1, 2);
		if (errCode != HAL_OK) return errCode;
	errCode = HAL_I2C_Master_Receive(&hi2c1, BQ25792_ADRESS, pData, len, 2);
	return errCode;
}

HAL_StatusTypeDef BQ25792_Write(uint8_t reg, uint8_t *pData, uint8_t len)
{
	if(len == 0 || len > 2 || pData == NULL)
		return HAL_ERROR;
	uint8_t Data[3];
	Data[0] = reg;
	Data[1] = pData[0];
	if(len >= 2) Data[2] = pData[1];
	return HAL_I2C_Master_Transmit(&hi2c1, BQ25792_ADRESS, Data, len, 2);
}
