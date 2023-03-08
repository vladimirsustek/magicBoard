/*
 * measurements.c
 *
 *  Created on: Mar 3, 2023
 *      Author: 42077
 */
#include <assert.h>

#include "measurements.h"

#include "adc.h"
#include "tim.h"

#define ADC_BUFFER_SIZE 16

/* */
const float V25 = 760; // V at 25°C
const float AVG_SLOPE = 2.5; //mV/°C
const float T_OFFSET = 25.0;
const uint32_t VDDA_NOM = 3300;
const uint32_t ADC_MAX = 4095;
const uint32_t ADC_CH_RANK_1 = 0;
const uint32_t ADC_CH_RANK_2 = 1;

static volatile uint32_t adc_buffer[ADC_BUFFER_SIZE];
static volatile uint32_t conv_done = 0;


#ifdef STM32F7
#if CALIBRATION_DATA_INTERPOLATION

	float TS_CAL1 = 0;
	float TS_CAL2 = 0;
	float V_REFIN_CAL = 0;

	/* Temperature sensor characteristics, datasheet - production data,
	 * STM32F765xx STM32F767xx STM32F768Ax STM32F769xx, 6.3.25 (Page 168) */
	/* TS ADC raw data acquired at temperature of 30 °C, VDDA= 3.3 V */
	TS_CAL1 = (float)*((uint16_t*)0x1FF0F44C); /* 0x1FF0F44C - 0x1FF0F44D*/
	/* TS ADC raw data acquired at temperature of 110 °C, VDDA= 3.3 V */

	TS_CAL2 = (float)*((uint16_t*)0x1FF0F44E); /* 0x1FF0F44E - 0x1FF0F44F*/
	/* Reference voltage, datasheet - production data,
	 * STM32F765xx STM32F767xx STM32F768Ax STM32F769xx, 6.3.27 (Page 169) */
	V_REFIN_CAL = (float)*((uint16_t*)0x1FF0F44A); /* 0x1FF0F44A - 0x1FF0F44B*/
#else

#endif
#else
#error UNSUPPORTED DOMAIN
#endif

void measurements_open()
{
	/* Start the timer and start a TIM-triggered endless DMA conversion */
	assert(HAL_OK == HAL_TIM_Base_Start(&htim4));
	assert(HAL_OK == HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, ADC_BUFFER_SIZE));
}

void measurement_close()
{
	/* Stop timer and the ADC */
	assert(HAL_OK == HAL_TIM_Base_Stop(&htim4));
	assert(HAL_OK == HAL_ADC_Stop(&hadc1));
}

bool measurement_get(int32_t* ch12, int32_t* temp)
{
	if(!conv_done)
	{
		return false;
	}
	else
	{
		HAL_NVIC_DisableIRQ(DMA2_Stream0_IRQn);
		conv_done = 0;
	}

	uint32_t ch12_acc = 0;
	uint32_t temp_acc = 0;

	assert(ch12 != NULL);
	assert(temp != NULL);

	/* Accumulating */
	for(uint32_t idx = 0; idx <= ADC_BUFFER_SIZE; idx+=2)
	{
		ch12_acc += adc_buffer[idx];
		temp_acc += adc_buffer[idx+1];
	}

	/* Averaging (when ADC_BUFFER_SIZE is 8)*/
	ch12_acc = ch12_acc / (ADC_BUFFER_SIZE/2);
	temp_acc = temp_acc / (ADC_BUFFER_SIZE/2);

	*ch12 = (ch12_acc*VDDA_NOM/ADC_MAX);

	/* Temperature sensor characteristics, RM0410 Reference manual,
	 * STM32F76xxx and STM32F77xxx ... , 15.10 (Page 464) */
	/*Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25*/
	/* TODO: use calibration values */

	/* Get voltage in mV*/
	temp_acc = temp_acc*VDDA_NOM/ADC_MAX;
	/* Get temperature in °C*/
	*temp = (int32_t)((((int32_t)temp_acc - V25)/AVG_SLOPE) + T_OFFSET);

	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

	return true;
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{

	if(hadc->Instance == ADC1)
	{
			conv_done = 1;
	}
}





