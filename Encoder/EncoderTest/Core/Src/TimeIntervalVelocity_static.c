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

volatile uint16_t prev_capture = 0;


void Encoder_PeroidMeas_Update(int16_t *result)
{
	uint16_t now = LL_TIM_IC_GetCaptureCH1(TIM4);
	uint16_t delta = now - prev_capture;
	prev_capture = now;

	if(delta == 0)
	{
		return;
	}

	*result = 60*Clock_Freq/(PulsesPerRevolution * delta);
}

