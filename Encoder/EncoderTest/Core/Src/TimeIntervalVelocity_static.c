/*
 * PeriodMeas.c
 *
 *  Created on: Mar 5, 2026
 *      Author: 1
 */
#include <TimeIntervalVelocity_static.h>
#include "main.h"

static const uint32_t Clock_Freq = 1000000;
static TIM_TypeDef* const Timer_Used = TIM4;
static const uint32_t capture_compare_channelA = LL_TIM_CHANNEL_CH1;
static const uint32_t capture_compare_channelB = LL_TIM_CHANNEL_CH2; // | LL_TIM_CHANNEL_CH2

static const uint8_t Encoder_Poles = 3;
static const uint8_t Encoder_Edges_Counted = 2;
static const uint8_t Encoder_Channels_Counted = 2;
static const uint16_t PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;

volatile uint16_t prev_capture = 0;
volatile int32_t prev_velocity = 0;
volatile uint16_t prev_num_pulses = 0;

void TIVs_Start()
{
	LL_TIM_SetCounter(Timer_Used, 0);

	LL_TIM_CC_EnableChannel(Timer_Used, capture_compare_channelA);
	LL_TIM_EnableIT_CC1(Timer_Used);

	if(Encoder_Channels_Counted == 2)
	{
		LL_TIM_CC_EnableChannel(Timer_Used, capture_compare_channelB);
		LL_TIM_EnableIT_CC2(Timer_Used);
	}

	LL_TIM_EnableCounter(Timer_Used);
	LL_TIM_GenerateEvent_UPDATE(Timer_Used);
	LL_TIM_ClearFlag_UPDATE(Timer_Used);

}

int32_t TIVs_CalculateVelocity(TIV_Channel_t channel)
{
	uint16_t current_capture = 0;

	if(channel == TIV_CHANNEL_A) LL_TIM_IC_GetCaptureCH1(Timer_Used);
	else LL_TIM_IC_GetCaptureCH2(Timer_Used);

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

