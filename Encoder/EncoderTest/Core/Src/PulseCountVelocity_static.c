/*
 * PulseMeas_static.c
 *
 *  Created on: Mar 6, 2026
 *      Author: pawluczenko
 */


//TO DO:
//cleanup startu
//dodanie oblusgi ISR

#include <PulseCountVelocity_static.h>

static const uint8_t Encoder_Poles = 3;
static const uint8_t Encoder_Edges_Counted = 2;
static const uint8_t Encoder_Channels_Counted = 1;
static const uint16_t Encoder_PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;

static const uint32_t Measurement_Frequency = 10;
static TIM_TypeDef* const Encoder_Timer = TIM1;
static TIM_TypeDef* const MeasurementFrame_Timer = TIM6;

static const uint8_t timeout_cycles_goal = 10;

static const uint32_t Measurement_Factor = (60*Measurement_Frequency)/Encoder_PulsesPerRevolution;

static volatile uint16_t old_num_pulses = 0;
static volatile uint8_t timeout_cycles = 0;
static volatile int32_t prev_velocity = 0;

int32_t PCVs_CalculateVelocity()
{
	uint16_t num_pulses = LL_TIM_GetCounter(Encoder_Timer);

	int16_t delta = (int16_t)(num_pulses - old_num_pulses);

	if(delta == 0) return 0;

	int32_t result = delta*Measurement_Factor;

	prev_velocity = result;
	old_num_pulses = num_pulses;

	return result;
}

int32_t PCVs_CalculateVelocity1()
{
	uint16_t num_pulses = LL_TIM_GetCounter(Encoder_Timer);

	int16_t delta = (int16_t)(num_pulses - old_num_pulses);

	if(delta == 0)
	{
		timeout_cycles++;
		if(timeout_cycles >= timeout_cycles_goal)
		{
			prev_velocity = 0;
			timeout_cycles = 0;
			return 0;
		}
		else
		{
			//prev_velocity = prev_velocity >> 1; // to chyba nie ma sensu ale zobaczym
			return prev_velocity;
		}
	}

	int32_t result;

	if(timeout_cycles == 0) result = delta*Measurement_Factor;
	else result = delta*Measurement_Factor/(1 + timeout_cycles);

	prev_velocity = result;
	old_num_pulses = num_pulses;
	timeout_cycles = 0;

	return result;
}

static void PCVs_EncoderTimer_Start()
{
	LL_TIM_SetCounter(Encoder_Timer, 0);
	LL_TIM_EnableCounter(Encoder_Timer);
}

static void PCVs_MeasFrameTimer_Start()
{
	LL_TIM_EnableCounter(MeasurementFrame_Timer);

	LL_TIM_GenerateEvent_UPDATE(MeasurementFrame_Timer);
	LL_TIM_ClearFlag_UPDATE(MeasurementFrame_Timer);

	LL_TIM_EnableIT_UPDATE(MeasurementFrame_Timer);
}

void PCVs_Start()
{
	PCVs_EncoderTimer_Start();
	PCVs_MeasFrameTimer_Start();
	old_num_pulses = 0;
}


