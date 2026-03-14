/*
 * Algorithm_profiler.c
 *
 *  Created on: Mar 12, 2026
 *      Author: pawluczenko
 */
#include "Algorithm_profiler.h"


static TIM_TypeDef* const Timer_used = TIM7;
static const uint32_t Timer_freq = 80000000;
static const uint32_t PSC = 0;
static const uint32_t ARR = 65535;

static const uint32_t tick_ns = 12; //12.5 tak naprawde dlatego potem jest korekcja
static uint32_t Profiler_execution_ns = 0;

void Profiler_Calibration()
{
	Profiler_Start();
	Profiler_execution_ns = Profiler_End();
}

void Profiler_Start()
{
	LL_TIM_GenerateEvent_UPDATE(Timer_used);
	LL_TIM_ClearFlag_UPDATE(Timer_used);
	LL_TIM_SetCounter(Timer_used, 0);
	LL_TIM_EnableCounter(Timer_used);
}

uint32_t Profiler_End()
{
	uint16_t ticks = LL_TIM_GetCounter(Timer_used);
	LL_TIM_DisableCounter(Timer_used);


	uint32_t correction = ticks/2;
	uint32_t ns_elapsed = tick_ns*ticks + correction - Profiler_execution_ns;
	return ns_elapsed;
}
