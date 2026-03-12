/*
 * Encoder.c
 *
 *  Created on: Feb 21, 2026
 *      Author: 1
 */

#include <PulseCountVelocity_object.h>



static uint16_t Calculate_pulsesPerRevolution(PCVo_Config_t* config)
{
	return (config -> Encoder_Channels_Counted)*(config->Encoder_Edges_Counted)*(config -> Encoder_Poles);
}
static uint32_t Calculate_measurementFactor(PCVo_Config_t* config)
{
	//return (60*(config -> Measurement_Frequency_hz))/Calculate_pulsesPerRevolution(config);
	return (60*10)/Calculate_pulsesPerRevolution(config);
}

int32_t PCVo_CalculateVelocity(PCVo_object* this)
{
	uint16_t num_pulses = LL_TIM_GetCounter(this -> timers -> Encoder_Timer);

	int16_t delta = (int16_t)(num_pulses - (this -> oldNumPulses));
	int32_t velocity = delta*(this -> Measurement_Factor1);
	this -> oldNumPulses = num_pulses;
	return velocity;
}

void PCVo_Init(PCVo_object* this,PCVo_Config_t* config,PCVo_Timers_t* timers)
{
	this -> config = config;
	this -> timers = timers;

	this -> oldNumPulses = LL_TIM_GetCounter(this -> timers -> Encoder_Timer);

	this -> Pulses_Per_Revolution = Calculate_pulsesPerRevolution(config);
	this -> Measurement_Factor1 = Calculate_measurementFactor(config);
}

void PCVo_Start(PCVo_object* this)
{
	LL_TIM_SetCounter(this -> timers -> Encoder_Timer, 0);

	LL_TIM_EnableCounter(this -> timers -> Encoder_Timer);
	LL_TIM_EnableCounter(this -> timers -> MeasurementFrame_Timer);

	LL_TIM_GenerateEvent_UPDATE(this -> timers -> MeasurementFrame_Timer);
	LL_TIM_GenerateEvent_UPDATE(this -> timers -> Encoder_Timer);
	LL_TIM_ClearFlag_UPDATE(this -> timers -> MeasurementFrame_Timer);
	LL_TIM_ClearFlag_UPDATE(this -> timers -> Encoder_Timer);
}





