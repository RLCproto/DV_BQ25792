#include "BQ25792.h"
#include "stm32l4xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef BQ25792_Init(void)
{
	uint8_t Data;




	Data =	BQ25792_ADC_CONTROL_EN |
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
	char pData[2];
	pData[0] = 0x00;

	len = sprintf(bufforTx,NCURSES_GOTOXY(1,2));
	HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	//VBAT
	if(BQ25792_Read(BQ25792_VBAT_ADC, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"VBAT: 0x%0.2x%0.2x\t%dmV\r\n", pData[0], pData[1], pData[0] << 8 | pData[1]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "VBAT: Hal error\r\n", 17, 10);

	//IBAT
	if(BQ25792_Read(BQ25792_IBAT_ADC, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"IBAT: 0x%0.2x%0.2x\t%dmA\r\n", pData[0], pData[1], pData[0] << 8 | pData[1]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "IBAT: Hal error\r\n", 17, 10);

	//VBUS
	if(BQ25792_Read(BQ25792_VBUS_ADC, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"VBUS: 0x%0.2x%0.2x\t%dmV\r\n", pData[0], pData[1], pData[0] << 8 | pData[1]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "VBUS: Hal error\r\n", 17, 10);

	//IBUS
	if(BQ25792_Read(BQ25792_IBUS_ADC, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"IBUS: 0x%0.2x%0.2x\t%dmA\r\n", pData[0], pData[1], pData[0] << 8 | pData[1]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "IBUS: Hal error\r\n", 17, 10);

	//VSYS
	if(BQ25792_Read(BQ25792_VSYS_ADC, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"VSYS: 0x%0.2x%0.2x\t%dmV\r\n", pData[0], pData[1], pData[0] << 8 | pData[1]);
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
	  len = sprintf(bufforTx,"Charge limit: 0x%0.2x%0.2x\r\n", pData[0], pData[1]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
	  HAL_UART_Transmit(&huart2, "Charge limit: Hal error\r\n", 25, 10);

	//Charger controls
	if(BQ25792_Read(BQ25792_CHARGER_CONTROL_0, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Chager control 0: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tEN_AUTO_IBATDIS: %x\r\n", (pData[0]&BQ25792_CHARGER_CONTROL_0_EN_AUTO_IBATDIS)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tFORCE_IBATDIS: %x\r\n", (pData[0]&BQ25792_CHARGER_CONTROL_0_FORCE_IBATDIS)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tEN_CHG: %x\r\n", (pData[0]&BQ25792_CHARGER_CONTROL_0_EN_CHG)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tEN_ICO: %x\r\n", (pData[0]&BQ25792_CHARGER_CONTROL_0_EN_ICO)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tFORCE_ICO: %x\r\n", (pData[0]&BQ25792_CHARGER_CONTROL_0_FORCE_ICO)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tEN_HIZ: %x\r\n", (pData[0]&BQ25792_CHARGER_CONTROL_0_EN_HIZ)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tEN_TERM: %x\r\n", (pData[0]&BQ25792_CHARGER_CONTROL_0_EN_TERM)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2,"Chager control 0: Hal error\r\n", 28, 10);

	//Charger statuses
	if(BQ25792_Read(BQ25792_CHARGER_STATUS_0, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Chager status 0: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tIINDPM_STAT: %x\r\n", (pData[0]&BQ25792_CHARGER_STATUS_0_IINDPM_STAT)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tVINDPM_STAT: %x\r\n", (pData[0]&BQ25792_CHARGER_STATUS_0_VINDPM_STAT)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tWD_STAT: %x\r\n", (pData[0]&BQ25792_CHARGER_STATUS_0_WD_STAT)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tPOORSRC_STAT: %x\r\n", (pData[0]&BQ25792_CHARGER_STATUS_0_POORSRC_STAT)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tPG_STAT: %x\r\n", (pData[0]&BQ25792_CHARGER_STATUS_0_PG_STAT)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tAC2_PRESENT_STAT: %x\r\n", (pData[0]&BQ25792_CHARGER_STATUS_0_AC2_PRESENT_STAT)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tAC1_PRESENT_STAT: %x\r\n", (pData[0]&BQ25792_CHARGER_STATUS_0_AC1_PRESENT_STAT)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  len = sprintf(bufforTx,"\tVBUS_PREWSENT_STAT: %x\r\n", (pData[0]&BQ25792_CHARGER_STATUS_0_VBUS_PRESENT_STAT)?1:0);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2,"Chager status 0: Hal error\r\n", 28, 10);

	if(BQ25792_Read(BQ25792_CHARGER_STATUS_1, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Chager status 1: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  switch(pData[0]&BQ25792_CHARGER_STATUS_1_CHG_STAT)
	  {
	  	  case (0<<5):
	  	  {
	  		  len = sprintf(bufforTx,"\tNot charging\r\n");
	  		  break;
	  	  }
	  	  case (1<<5):
	  	  {
	  		  len = sprintf(bufforTx,"\tTrickle charging\r\n");
	  		  break;
	  	  }
	  	  case (2<<5):
	  	  {
	  		  len = sprintf(bufforTx,"\tPre-charge\r\n");
	  		  break;
	  	  }
	  	  case (3<<5):
	  	  {
	  		  len = sprintf(bufforTx,"\tFast charge (CC)\r\n");
	  		  break;
	  	  }
	  	  case (4<<5):
	  	  {
	  		  len = sprintf(bufforTx,"\tTaper charge (CV)\r\n");
	  		  break;
	  	  }
	  	  case (6<<5):
	  	  {
	  		  len = sprintf(bufforTx,"\tNot charging\r\n");
	  		  break;
	  	  }
	  	  case (7<<5):
	  	  {
	  		  len = sprintf(bufforTx,"\tNot charging\r\n");
	  		  break;
	  	  }
	  	  default:
	  	  {
	  		  len = sprintf(bufforTx,"\t%sCHARGER_STATUS_1 flag mask_CHG_STAT error%s\r\n",NCURSES_BG_RED, NCURSES_BG_DEFAULT);
	  	  }
	  }
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  switch(pData[0]&BQ25792_CHARGER_STATUS_1_VBUS_STAT)
	  {
		  case (0<<1):
		  {
			  len = sprintf(bufforTx,"\tNo input\r\n");
			  break;
		  }
		  case (1<<1):
		  {
			  len = sprintf(bufforTx,"\tUSB SDP (500mA)\r\n");
			  break;
		  }
		  case (2<<1):
		  {
			  len = sprintf(bufforTx,"\tUSB CDP (1.5A)\r\n");
			  break;
		  }
		  case (3<<1):
		  {
			  len = sprintf(bufforTx,"\tUSB DCP (3.25A)\r\n");
			  break;
		  }
		  case (4<<1):
		  {
			  len = sprintf(bufforTx,"\tAdjustable High Voltage DCP (HVDCP) (1.5A)\r\n");
			  break;
		  }
		  case (5<<1):
		  {
			  len = sprintf(bufforTx,"\tUnknown adaptor (3A)\r\n");
			  break;
		  }
		  case (6<<1):
		  {
			  len = sprintf(bufforTx,"\tNon-Standard Adapter (1A/2A/2.1A/2.4A)\r\n");
			  break;
		  }
		  case (7<<1):
		  {
			  len = sprintf(bufforTx,"\tIn OTG mode\r\n");
			  break;
		  }
		  case (8<<1):
		  {
			  len = sprintf(bufforTx,"\tNot qualified adaptor\r\n");
			  break;
		  }
		  case (0xB<<1):
		  {
			  len = sprintf(bufforTx,"\tDevice directly powered from VBUS\r\n");
			  break;
		  }
	  	  default:
	  	  {
	  		  len = sprintf(bufforTx,"\t%sCHARGER_STATUS_1_VBUS_STAT flag mask error%s\r\n",NCURSES_BG_RED, NCURSES_BG_DEFAULT);
	  	  }
	  }
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_1_BC12_DONE_STAT)?"BC1.2 or non-standard detection complete":"BC1.2 or non-standard detection NOT complete");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2,"Chager status 1: Hal error\r\n", 28, 10);

	if(BQ25792_Read(BQ25792_CHARGER_STATUS_2, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Chager status 2: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  switch(pData[0] & BQ25792_CHARGER_STATUS_2_ICO_STAT)
	  {
	  	case (0<<6):
		{
	  	  len = sprintf(bufforTx,"\tICO disabled\r\n");
		  break;
		}
	  	case (1<<6):
		{
	  	  len = sprintf(bufforTx,"\tICO optimization in progress\r\n", pData[0]);
		  break;
		}
	  	case (2<<6):
		{
	  	  len = sprintf(bufforTx,"\tMaximum input current detected\r\n", pData[0]);
		  break;
		}
	  	default:
	  	{
	  		  len = sprintf(bufforTx,"\t%sCHARGER_STATUS_2_ICO_STAT flag mask error%s\r\n",NCURSES_BG_RED, NCURSES_BG_DEFAULT);
	  	}
	  }
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_2_TREG_STAT)?"Device in thermal regulation":"Device in nominal thermal zone");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_2_DPDM_STAT)?"The D+/D- detection is ongoing":"The D+/D- detection is NOT started yet, or the detection is done");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_2_VBAT_PRESENT_STAT)?"VBAT present":"VBAT NOT present");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2, "Chager status 2: Hal error\r\n", 28, 10);

	if(BQ25792_Read(BQ25792_CHARGER_STATUS_3, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Chager status 3: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_3_ACRB2_STAT)?"ACFET2-RBFET2 is placed":"ACFET2-RBFET2 is NOT placed");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_3_ACRB1_STAT)?"ACFET1-RBFET1 is placed":"ACFET1-RBFET1 is NOT placed");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_3_ADC_DONE_STAT)?"Conversion complete":"Conversion NOT complete");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_3_VSYS_STAT)?"In VSYSMIN regulation (VBAT < VSYSMIN)":"Not in VSYSMIN regulation (VBAT > VSYSMIN)");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_3_CHG_TMR_STAT)?"Fast charge safety timer expired":"Fast charge safety timer OK");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_3_TRICHG_TMR_STAT)?"Trickle charge safety timer expired":"Trickle charge safety timer OK");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_3_PRECHG_TMR_STAT)?"Pre-charge safety timer expired":"Pre-charge safety timer OK");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2, "Chager status 3: Hal error\r\n", 28, 10);

	if(BQ25792_Read(BQ25792_CHARGER_STATUS_4, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Charger status 4: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  len = sprintf(bufforTx,"\t%s\r\n", (pData[0]&BQ25792_CHARGER_STATUS_4_VBATOTG_LOW_STAT)?"The battery voltage is too low to enable the OTG operation":"The battery voltage is high enough to enable the OTG operation");
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  switch(pData[0]&(BQ25792_CHARGER_STATUS_4_TS_COLD_STAT | BQ25792_CHARGER_STATUS_4_TS_COOL_STAT | BQ25792_CHARGER_STATUS_4_TS_WARM_STAT | BQ25792_CHARGER_STATUS_4_TS_HOT_STAT))
	  {
	  	  case BQ25792_CHARGER_STATUS_4_TS_COLD_STAT:
	  	  {
	  		  len = sprintf(bufforTx,"\t%sTemperature sensor in clod range%s\r\n", NCURSES_CLRL, NCURSES_CLRR);
	  		  break;
	  	  }
	  	  case BQ25792_CHARGER_STATUS_4_TS_COOL_STAT:
	  	  {
	  		  len = sprintf(bufforTx,"\t%sTemperature sensor in cool range%s\r\n", NCURSES_CLRL, NCURSES_CLRR);
	  		  break;
	  	  }
	  	  case BQ25792_CHARGER_STATUS_4_TS_WARM_STAT:
	  	  {
	  		  len = sprintf(bufforTx,"\t%sTemperature sensor in warm range%s\r\n", NCURSES_CLRL, NCURSES_CLRR);
	  		  break;
	  	  }
	  	  case BQ25792_CHARGER_STATUS_4_TS_HOT_STAT:
	  	  {
	  		  len = sprintf(bufforTx,"\t%sTemperature sensor in hot range%s\r\n", NCURSES_CLRL, NCURSES_CLRR);
	  		  break;
	  	  }
	  	  default:
	  	  {
	  		  len = sprintf(bufforTx,"\t%sTemperature sensor in multiple range: %x%s\r\n", NCURSES_BG_RED, pData[0]&(BQ25792_CHARGER_STATUS_4_TS_COLD_STAT | BQ25792_CHARGER_STATUS_4_TS_COOL_STAT | BQ25792_CHARGER_STATUS_4_TS_WARM_STAT | BQ25792_CHARGER_STATUS_4_TS_HOT_STAT), NCURSES_BG_DEFAULT);
	  	  }
	  }
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	}
	else
		HAL_UART_Transmit(&huart2, "Chager status 4: Hal error\r\n", 28, 10);

	//Fault statuses
	if(BQ25792_Read(BQ25792_FAULT_STATUS_0, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Fault status 0: 0x%0.2x%s\r\n", pData[0], NCURSES_CLRR);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  if(pData[0] & BQ25792_FAULT_STATUS_0_IBAT_REG_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in battery discharging current regulation%s%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT, NCURSES_CLRR);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_0_VBUS_OVP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in over VBUS voltage protection%s%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT, NCURSES_CLRR);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_0_VBAT_OVP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in over battery voltage protection%s%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT, NCURSES_CLRR);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_0_IBUS_OCP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in over IBUS current protection%s%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT, NCURSES_CLRR);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_0_IBAT_OCP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in over IBAT current protection%s%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT, NCURSES_CLRR);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_0_CONV_OCP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sConverter in over current protection%s%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT, NCURSES_CLRR);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_0_VAC2_OVP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in over AVC2 voltage protection%s%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT, NCURSES_CLRR);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_0_VAC1_OVP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in over AVC2 voltage protection%s%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT, NCURSES_CLRR);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	}
	else
		HAL_UART_Transmit(&huart2, "Fault status 0: Hal error\r\n", 27, 10);

	if(BQ25792_Read(BQ25792_FAULT_STATUS_1, pData, 1) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Fault status 1: 0x%0.2x%s\r\n", pData[0], NCURSES_CLRR);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  if(pData[0] & BQ25792_FAULT_STATUS_1_VSYS_SHORT_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in SYS short circuit protection%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_1_VSYS_OVP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in SYS over-voltage protection%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_1_OTG_OVP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in OTG over-voltage%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_1_OTG_UVP_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in OTG under voltage%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0] & BQ25792_FAULT_STATUS_1_TSHUT_STAT)
	  {
		  len = sprintf(bufforTx,"\t%s%sDevice in thermal shutdown protection%s\r\n", NCURSES_CLRL, NCURSES_BG_RED, NCURSES_BG_DEFAULT);
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	}
	else
		HAL_UART_Transmit(&huart2, "Fault status 1: Hal error\r\n", 27, 10);

#if 0 //informacja powielona w statusie
	//Charger flags
	if(BQ25792_Read(BQ25792_CHARGER_FLAG_0, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Charger flag 0: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  if(pData[0]&BQ25792_CHARGER_FLAGS_0_IINDPM_FLAG)
	  {
		  len = sprintf(bufforTx,"\tIINDPM / IOTG signal rising edge detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_0_VINDPM_FLAG)
	  {
		  len = sprintf(bufforTx,"\tVINDPM / VOTG regulation signal rising edge detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_0_WD_FLAG)
	  {
		  len = sprintf(bufforTx,"\tWD timer signal rising edge detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_0_POORSRC_FLAG)
	  {
		  len = sprintf(bufforTx,"\tPoor source status rising edge detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_0_PG_FLAG)
	  {
		  len = sprintf(bufforTx,"\tAny change in PG_STAT even\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_0_AC2_PRESENT_FLAG)
	  {
		  len = sprintf(bufforTx,"\tVAC2 present status changed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_0_AC1_PRESENT_FLAG)
	  {
		  len = sprintf(bufforTx,"\tVAC1 present status changed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_0_VBUS_PRESENT_FLAG)
	  {
		  len = sprintf(bufforTx,"\tVBUS present status changed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	}
	else
	  HAL_UART_Transmit(&huart2, "Charger flag 0: Hal error\r\n", 27, 10);

	if(BQ25792_Read(BQ25792_CHARGER_FLAG_1, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Charger flag 1: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  if(pData[0]&BQ25792_CHARGER_FLAGS_1_CHG_FLAG)
	  {
		  len = sprintf(bufforTx,"\tCharge status changed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_1_ICO_FLAG)
	  {
		  len = sprintf(bufforTx,"\tICO status changed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_1_VBUS_FLAG)
	  {
		  len = sprintf(bufforTx,"\tVBUS status changed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_1_TREG_FLAG)
	  {
		  len = sprintf(bufforTx,"\tTREG signal rising threshold detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_1_VBAT_PRESENT_FLAG)
	  {
		  len = sprintf(bufforTx,"\tVBAT present status changed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_1_BC12_DONE_FLAG)
	  {
		  len = sprintf(bufforTx,"\tBC1.2 detection status changed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	}
	else
	  HAL_UART_Transmit(&huart2, "Charger flag 1: Hal error\r\n", 27, 10);

	if(BQ25792_Read(BQ25792_CHARGER_FLAG_2, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Charger flag 2: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  if(pData[0]&BQ25792_CHARGER_FLAGS_2_DPDM_DONE_FLAG)
	  {
		  len = sprintf(bufforTx,"\tD+/D- detection is completed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_2_ADC_DONE_FLAG)
	  {
		  len = sprintf(bufforTx,"\tConversion completed\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_2_VSYS_FLAG)
	  {
		  len = sprintf(bufforTx,"\tEntered or existed VSYSMIN regulation\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_2_CHG_TMR_FLAG)
	  {
		  len = sprintf(bufforTx,"\tFast charge timer expired rising edge detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_2_TRICHG_TMR_FLAG)
	  {
		  len = sprintf(bufforTx,"\tTrickle charger timer expired rising edge detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_2_PRECHG_TMR_FLAG)
	  {
		  len = sprintf(bufforTx,"\tPre-charge timer expired rising edge detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_2_TOPOFF_TMR_FLAG)
	  {
		  len = sprintf(bufforTx,"\tTop off timer expired rising edge detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	}
	else
	  HAL_UART_Transmit(&huart2, "Charger flag 2: Hal error\r\n", 27, 10);

	if(BQ25792_Read(BQ25792_CHARGER_FLAG_3, pData, 2) == HAL_OK)
	{
	  len = sprintf(bufforTx,"Charger flag 3: 0x%0.2x\r\n", pData[0]);
	  HAL_UART_Transmit(&huart2, bufforTx, len, 10);

	  if(pData[0]&BQ25792_CHARGER_FLAGS_3_VBATOTG_LOW_FLAG)
	  {
		  len = sprintf(bufforTx,"\tVBAT falls below the threshold to enable the OTG mode\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_3_TS_COLD_FLAG)
	  {
		  len = sprintf(bufforTx,"\tTS across cold temperature (T1) is detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_3_TS_COOL_FLAG)
	  {
		  len = sprintf(bufforTx,"\tTS across cool temperature (T2) is detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_3_TS_WARM_FLAG)
	  {
		  len = sprintf(bufforTx,"\tTS across warm temperature (T3) is detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	  if(pData[0]&BQ25792_CHARGER_FLAGS_3_TS_HOT_FLAG)
	  {
		  len = sprintf(bufforTx,"\tTS across hot temperature (T5) is detected\r\n");
		  HAL_UART_Transmit(&huart2, bufforTx, len, 10);
	  }
	}
	else
	  HAL_UART_Transmit(&huart2, "Charger flag 1: Hal error\r\n", 27, 10);

	//fault flags
#endif

	HAL_UART_Transmit(&huart2, NCURSES_CLRLINE, 4, 10);
}
