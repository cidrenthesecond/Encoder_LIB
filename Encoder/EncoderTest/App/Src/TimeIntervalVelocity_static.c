/*
 * PeriodMeas.c
 *
 *  Created on: Mar 5, 2026
 *      Author: 1
 */

//TO DO
//Adidtion of low pass filter
//Problems
//at irregular intervals Tivs_calculatevelocity returns near 0 or 0 value

#include <TimeIntervalVelocity_static.h>
#include "main.h"
#include "Ease_of_life.h"


//CONFIG START:
static const uint32_t Clock_Freq = 1000000;
static TIM_TypeDef* const Timer_Used = TIM4;
static const uint32_t capture_compare_channelA = LL_TIM_CHANNEL_CH1;
static const uint32_t capture_compare_channelB = LL_TIM_CHANNEL_CH2; // | LL_TIM_CHANNEL_CH2
static GPIO_TypeDef* const cc_ChannelA_gpio_port = TIM4_CH1_GPIO_Port;
static const uint32_t cc_ChannelA_pin = TIM4_CH1_Pin;
static GPIO_TypeDef* const cc_ChannelB_gpio_port = TIM4_CH2_GPIO_Port;
static const uint32_t cc_ChannelB_pin = TIM4_CH2_Pin;

static const uint8_t Encoder_Poles = 3;
static const uint8_t Encoder_Edges_Counted = 2;
static const uint8_t Encoder_Channels_Counted = 2;

static const uint8_t timeout_cycles_goal = 10;
//CONFIG END

static const uint16_t PulsesPerRevolution = Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted;


static volatile uint16_t prev_capture = 0;
static volatile int32_t prev_velocity = 0;
static volatile uint16_t prev_num_pulses = 0;
static volatile uint8_t timeout_cycles = 0;

void TIVs_Start()
{
	LL_TIM_SetCounter(Timer_Used, 0);

	LL_TIM_CC_EnableChannel(Timer_Used, capture_compare_channelA);
	LL_TIM_EnableIT_CC(Timer_Used, capture_compare_channelA);

	if(Encoder_Channels_Counted == 2)
	{
		LL_TIM_CC_EnableChannel(Timer_Used, capture_compare_channelB);
		LL_TIM_EnableIT_CC(Timer_Used, capture_compare_channelB);
	}

	LL_TIM_EnableCounter(Timer_Used);
	LL_TIM_GenerateEvent_UPDATE(Timer_Used);
	LL_TIM_ClearFlag_UPDATE(Timer_Used);

	LL_TIM_EnableIT_UPDATE(Timer_Used);
}

int32_t TIVs_TimerOverflowISR() {
	timeout_cycles++;

	if(timeout_cycles >= timeout_cycles_goal)
	{
		prev_velocity = 0;
		return 0;
	}

	return prev_velocity;
}

static inline uint8_t Are_pin_states_equal()
{
    return LL_GPIO_GetState(cc_ChannelA_gpio_port, cc_ChannelA_pin) ==
           LL_GPIO_GetState(cc_ChannelB_gpio_port, cc_ChannelB_pin);
}

static inline int32_t TIVs_GetSign(uint32_t cc_channelx)
{
    uint8_t equal = Are_pin_states_equal();

    if(cc_channelx == cc_ChannelA_pin)
        return equal ? 1 : -1;

    if(cc_channelx == cc_ChannelB_pin)
        return equal ? -1 : 1;

    return 0;
}

int32_t TIVs_CalculateVelocity(TIV_Channel_t channel)
{
	uint16_t current_capture = 0;
	const uint8_t timeout_num = timeout_cycles;

	int32_t sign;
	if(channel == TIV_CHANNEL_A) sign = TIVs_GetSign(cc_ChannelA_pin);
	else sign = TIVs_GetSign(cc_ChannelB_pin);

	if(channel == TIV_CHANNEL_A) current_capture = LL_TIM_IC_GetCaptureCH1(Timer_Used);
	else current_capture = LL_TIM_IC_GetCaptureCH2(Timer_Used);


	uint32_t delta = 65535*timeout_num  + current_capture - prev_capture;

	prev_capture = current_capture;

	if(delta == 0 )
	{
		return prev_velocity;
	}

	int32_t result;

	result = 60*Clock_Freq/(PulsesPerRevolution * delta);

	result = sign*result;

	prev_velocity = result;
	timeout_cycles = 0;

	return result;
}

//	EXAMPLE OF ISR FUNCTION
//	MIND CC1 clear function

/*
	void TIMx_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_CC1(Timer_Used))
	{
		LL_TIM_ClearFlag_CC1(TIM4);
		TIV_result = TIVs_CalculateVelocity(TIV_CHANNEL_A);
	}

	//if two channels are used

	if(LL_TIM_IsActiveFlag_CC2(Timer_Used))
	{
		LL_TIM_ClearFlag_CC2(TIM4);
		TIV_result = TIVs_CalculateVelocity(TIV_CHANNEL_B);
	}

	//if timeout expection and longer measurement frame is used

	if(LL_TIM_IsActiveFlag_UPDATE(Timer_Used) && LL_TIM_IsEnabledIT_UPDATE(TIM4))
	{
		LL_TIM_ClearFlag_UPDATE(TIM4);
		TIV_result = TIVs_TimerOverflowISR();
	}
}
 */



