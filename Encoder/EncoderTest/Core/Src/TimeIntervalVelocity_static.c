/*
 * PeriodMeas.c
 *
 *  Created on: Mar 5, 2026
 *      Author: 1
 */
#include <TimeIntervalVelocity_static.h>
#include "main.h"

static const uint32_t Clock_Freq = 1000000;
static const uint32_t PulsesPerRevolution = 3;
static TIM_TypeDef* const Timer_Used = TIM4;

volatile uint16_t prev_capture = 0;
volatile int32_t prev_velocity = 0;

int32_t TIV_CalculateVelocity()
{
	uint16_t current_capture = LL_TIM_IC_GetCaptureCH1(Timer_Used);
	uint16_t delta = current_capture - prev_capture;
	prev_capture = current_capture;

	if(delta == 0)
	{
		return prev_velocity;
	}

	int32_t result = 60*Clock_Freq/(PulsesPerRevolution * delta);
	prev_velocity = result;
	return result;
}

