/*
 * HybridVelocity.c
 *
 *  Created on: Mar 11, 2026
 *      Author: 1
 */

#include "main.h"

//TO DO :mozna zmniejszyc do jednego timera pomyslec nad tym

static const uint32_t Clock_Freq = 1000000;
static TIM_TypeDef* const capture_compare_timer = TIM4;
static const uint32_t capture_compare_channelA = LL_TIM_CHANNEL_CH1;
static const uint32_t capture_compare_channelB = LL_TIM_CHANNEL_CH2; // | LL_TIM_CHANNEL_CH2

static TIM_TypeDef* Encoder_timer = TIM1;

static const uint8_t Encoder_Poles = 3;
static const uint8_t Encoder_Edges_Counted = 2;
static const uint8_t Encoder_Channels_Counted = 2;
static const uint16_t PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;
static const uint32_t measurement_factor = 60 * Clock_Freq / PulsesPerRevolution;

static const uint8_t timeout_cycles_goal = 10;

void HV_Start()
{
	LL_TIM_SetCounter(Encoder_timer, 0);
	LL_TIM_EnableCounter(Encoder_timer);

	LL_TIM_SetCounter(capture_compare_timer, 0);
	LL_TIM_CC_EnableChannel(capture_compare_timer, capture_compare_channelA);
	LL_TIM_EnableIT_CC1(capture_compare_timer);

	if(Encoder_Channels_Counted == 2)
	{
		LL_TIM_CC_EnableChannel(capture_compare_timer, capture_compare_channelB);
		LL_TIM_EnableIT_CC2(capture_compare_timer);
	}

	LL_TIM_EnableCounter(capture_compare_timer);
	LL_TIM_GenerateEvent_UPDATE(capture_compare_timer);
	LL_TIM_ClearFlag_UPDATE(capture_compare_timer);

	LL_TIM_EnableIT_UPDATE(capture_compare_timer);
}

static volatile uint16_t lastcapture = 0;
static volatile uint16_t lastcapture_previous = 0;
static volatile uint8_t timeout_cycles = 0;
static volatile uint8_t timeout_flag = 0;
static volatile uint8_t old_num_pulses = 0;
static volatile int32_t prev_velocity = 0;

void HV_CaptureCompare_ISR()
{
	if(LL_TIM_IsActiveFlag_CC1(capture_compare_timer))
	{
		LL_TIM_ClearFlag_CC1(capture_compare_timer);
		lastcapture = LL_TIM_IC_GetCaptureCH1(capture_compare_timer);
	}
	if(LL_TIM_IsActiveFlag_CC2(capture_compare_timer))
	{
		LL_TIM_ClearFlag_CC2(capture_compare_timer);
		lastcapture = LL_TIM_IC_GetCaptureCH2(capture_compare_timer);
	}
	if(LL_TIM_IsActiveFlag_UPDATE(capture_compare_timer) && LL_TIM_IsEnabledIT_UPDATE(capture_compare_timer))
	{
		LL_TIM_ClearFlag_UPDATE(capture_compare_timer);

		timeout_cycles++;
		if(timeout_cycles >= timeout_cycles_goal)
		{
			timeout_cycles = 0;
			timeout_flag = 1;
		}
	}
}



int32_t HV_CalculateVelocity()
{

	uint16_t num_pulses = LL_TIM_GetCounter(Encoder_timer);
	int16_t delta = (int16_t)(num_pulses - old_num_pulses);
	uint16_t capture = lastcapture; //trzeba przechowac przed zmiana

	if(delta == 0)
	{
		timeout_cycles++;

		if(timeout_cycles >= timeout_cycles_goal)
		{
			prev_velocity = 0;
			return 0;
		}
	}

	if(prev_velocity == 0) timeout_cycles = 0;

	uint32_t time_frame = 65536*timeout_cycles + lastcapture_previous - capture;

	int32_t result = delta * measurement_factor / time_frame;

	lastcapture_previous = lastcapture;
	prev_velocity = result;
	old_num_pulses = num_pulses;

	return result;
}
