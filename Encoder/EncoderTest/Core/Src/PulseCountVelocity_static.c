/*
 * PulseMeas_static.c
 *
 *  Created on: Mar 6, 2026
 *      Author: pawluczenko
 */
#include <PulseCountVelocity_static.h>

static const uint8_t Encoder_Poles = 3;
static const uint8_t Encoder_Edges_Counted = 2;
static const uint8_t Encoder_Channels_Counted = 1;
static const uint16_t Encoder_PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;

static const uint32_t Measurement_Frequency = 10;
static TIM_TypeDef* const Encoder_Timer = TIM1;
static TIM_TypeDef* const MeasurementFrame_Timer = TIM6;

static const uint32_t Measurement_Factor = (60*Measurement_Frequency)/Encoder_PulsesPerRevolution;

static uint16_t old_num_pulses = 0;

int32_t PCVs_CalculateVelocity()
{
	uint16_t num_pulses = LL_TIM_GetCounter(Encoder_Timer);

	//LL_TIM_SetCounter(TIM1, 0);

	int16_t delta = (int16_t)(num_pulses - old_num_pulses);
	int32_t result = delta*Measurement_Factor;
	old_num_pulses = num_pulses;
	return result;
}

void PCVs_Start()
{
	LL_TIM_SetCounter(Encoder_Timer, 0);

	LL_TIM_EnableCounter(Encoder_Timer);
	LL_TIM_EnableCounter(MeasurementFrame_Timer);

	LL_TIM_GenerateEvent_UPDATE(MeasurementFrame_Timer);
	LL_TIM_ClearFlag_UPDATE(MeasurementFrame_Timer);

	LL_TIM_EnableIT_UPDATE(MeasurementFrame_Timer);

	old_num_pulses = 0;
}
