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

static const uint8_t Encoder_Poles = 3;
static const uint8_t Encoder_Edges_Counted = 2;
static const uint8_t Encoder_Channels_Counted = 2;
static const uint16_t PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;
static const uint16_t measurement_factor = 60 / PulsesPerRevolution;

static const uint32_t Measurement_Frequency = 10;
static TIM_TypeDef* const MeasurementFrame_Timer = TIM6;

static TIM_TypeDef* const Encoder_Timer = TIM1;

void HV_Start()
{
	LL_TIM_SetCounter(Encoder_Timer, 0);
	LL_TIM_EnableCounter(Encoder_Timer);

	LL_TIM_EnableCounter(MeasurementFrame_Timer);
	LL_TIM_GenerateEvent_UPDATE(MeasurementFrame_Timer);
	LL_TIM_ClearFlag_UPDATE(MeasurementFrame_Timer);
	LL_TIM_EnableIT_UPDATE(MeasurementFrame_Timer);

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

	//LL_TIM_EnableIT_UPDATE(capture_compare_timer);
}

static uint16_t lastcapture = 0;

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
}



int32_t HV_CalculateVelocity()
{
	;
}
