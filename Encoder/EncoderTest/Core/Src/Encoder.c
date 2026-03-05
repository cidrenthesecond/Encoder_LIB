/*
 * Encoder.c
 *
 *  Created on: Feb 21, 2026
 *      Author: 1
 */

#include "main.h"
#include "encoder.h"

uint8_t Calculate_pulsesPerRevolution(Encoder_Info_t* info)
{
	return (info -> Encoder_Channels_Counted1)*(info->Encoder_Edges_Counted1)*(info -> Encoder_Poles1);
}

uint32_t Calculate_MeasurementFactor(Encoder_Info_t* info)
{
	return Calculate_pulsesPerRevolution(info)*(info -> Measurement_Frequency1)/60 ;
}

int16_t CalculateVelocity(struct Encoder_PulseMeas* this)
{
	int16_t num_pulses = (int16_t)LL_TIM_GetCounter(this -> Encoder_Info.Tim_Used);

	int16_t delta = num_pulses - (this -> oldNumPulses);
	int16_t velocity = (int16_t)(delta*(this -> Measurement_Factor1));
	this -> oldNumPulses = num_pulses;
	return velocity;
}

void Encoder_Init(struct Encoder_PulseMeas* this,Encoder_Info_t* info)
{
	this -> Encoder_Info.Tim_Used = info -> Tim_Used;
	this -> Encoder_Info.Measurement_Frequency1 = info -> Measurement_Frequency1;
	this -> Encoder_Info.Encoder_Poles1 = info -> Encoder_Poles1;
	this -> Encoder_Info.Encoder_Edges_Counted1 = info -> Encoder_Edges_Counted1;
	this -> Encoder_Info.Encoder_Channels_Counted1 = info -> Encoder_Edges_Counted1;

	this -> oldNumPulses = LL_TIM_GetCounter(this -> Encoder_Info.Tim_Used);

	this -> Pulses_Per_Revolution = Calculate_pulsesPerRevolution(info);
	this -> Measurement_Factor1 = Calculate_measurementFactor(info);

	this -> CalculateVelocity = *CalculateVelocity(this);
}

static volatile int16_t old_num_pulses = 0;

Encoder_Status_t Encoder_PulseMeas_Update(int16_t* result)
{
	int16_t num_pulses = (int16_t)LL_TIM_GetCounter(TIM1);

	//LL_TIM_SetCounter(TIM1, 0);

	int16_t delta = num_pulses - old_num_pulses;
	*result = (int16_t)(delta*Measurement_Factor);
	old_num_pulses = num_pulses;
	return OK;
}

volatile Encoder_PulseMeas_Ctx_t* M_Method_Measurement = {0};

Encoder_Status_t Encoder_PulseMeas_Update1(Encoder_PulseMeas_Ctx_t* ctx)
{
	ctx -> pulses = (int16_t)LL_TIM_GetCounter(TIM1);

	if( ctx -> pulses == ctx -> old_pulses)
	{
		ctx -> callbacks_elapsed++;
		return NO_PULSE;
	}

	int16_t delta = (ctx -> pulses) - (ctx -> old_pulses);
	ctx -> result = (int16_t)(delta*Measurement_Factor);
	ctx -> old_pulses = ctx -> pulses;
	return OK;
}


static volatile uint16_t prev_capture = 0;

#define Clock_Freq	1000000
const int Clock_Freq1 = 1;
unsigned int miau;
#define Revolution  3

Encoder_Status_t Encoder_PeroidMeas_Update(int16_t *result)
{
	uint16_t now = LL_TIM_IC_GetCaptureCH1(TIM4);
	uint16_t delta = now - prev_capture;
	prev_capture = now;

	if(delta == 0)
	{
		return NO_PULSE;
	}

	*result = 60*Clock_Freq/(Revolution * delta);
	return OK;
}



