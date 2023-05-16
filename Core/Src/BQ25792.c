#include "BQ25792.h"
#include "stm32l4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef BQ25792_Init(void)
{
	uint8_t Data =	BQ25792_ADC_CONTROL_EN |
					BQ25792_ADC_CONTROL_RATE_CONTINUOUS |
					BQ25792_ADC_CONTROL_SAMPLE_15B;

	return BQ25792_Write(BQ25792_ADC_CONTROL, &Data, 1);
}

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


#include "ncurses.h"
void BQ25792_Debug(void)
{
	extern UART_HandleTypeDef huart2;
	char bufforTx[128];
	int len;
	char pData[16];
	pData[0] = 0x00;

	len = sprintf(bufforTx,NCURSES_GOTOXY(1,2));
	HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	//VBAT
	if(BQ25792_Read(BQ25792_VBAT_ADC, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"VBAT: 0x%0.2x%0.2x\r\n", pData[0], pData[1]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "VBAT: Hal error\r\n", 17, 10);

	//VBUS
	if(BQ25792_Read(BQ25792_VBUS_ADC, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"VBUS: 0x%0.2x%0.2x\r\n", pData[0], pData[1]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "VBUS: Hal error\r\n", 17, 10);

	//VSYS
	if(BQ25792_Read(BQ25792_VSYS_ADC, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"VSYS: 0x%0.2x%0.2x\r\n", pData[0], pData[1]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "VSYS: Hal error\r\n", 17, 10);

	//ADC control
	if(BQ25792_Read(BQ25792_ADC_CONTROL, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"ADC control: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "ADC con: Hal error\r\n", 17, 10);

	//ADC functions
	if(BQ25792_Read(BQ25792_ADC_FUNCTION_DISABLE_0, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"ADC fun0: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "ADC fun: Hal error\r\n", 20, 10);
	if(BQ25792_Read(BQ25792_ADC_FUNCTION_DISABLE_1, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"ADC fun1: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "ADC fun: Hal error\r\n", 20, 10);

	//Upper voltage limit
	if(BQ25792_Read(BQ25792_CHARGE_VOLTAGE_LIMIT, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Cha lim: 0x%0.2x%0.2x\r\n", pData[0], pData[1]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "Cha lim: Hal error\r\n", 20, 10);

	//Charger status
	if(BQ25792_Read(BQ25792_CHARGER_STATUS_0, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Cha sta0: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2, "Cha sta0: Hal error\r\n", 21, 10);
	if(BQ25792_Read(BQ25792_CHARGER_STATUS_1, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Cha sta1: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2, "Cha sta1: Hal error\r\n", 21, 10);
	if(BQ25792_Read(BQ25792_CHARGER_STATUS_2, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Cha sta2: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2, "Cha sta2: Hal error\r\n", 21, 10);
	if(BQ25792_Read(BQ25792_CHARGER_STATUS_3, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Cha sta3: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2, "Cha sta3: Hal error\r\n", 21, 10);
	if(BQ25792_Read(BQ25792_CHARGER_STATUS_4, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Cha sta4: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2, "Cha sta4: Hal error\r\n", 21, 10);
}
