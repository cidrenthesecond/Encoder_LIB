/*
 * encoder.h
 *
 *  Created on: Feb 24, 2026
 *      Author: 1
 */

#ifndef INC_PULSECOUNTVELOCITY_OBJECT_H_
#define INC_PULSECOUNTVELOCITY_OBJECT_H_

#include "main.h"

/*

for this method to work you need:

*Timer configured as clock to generate periodical interrupts
*Timer configured in encoder mode to count impulses genereted
 in given time

*/
typedef struct {
	uint32_t Measurement_Frequency_hz;
	uint8_t Encoder_Poles;
	uint8_t Encoder_Edges_Counted;
	uint8_t Encoder_Channels_Counted;
}PCVo_Config_t;

typedef struct {
	TIM_TypeDef *Encoder_Timer;
	TIM_TypeDef *MeasurementFrame_Timer;
}PCVo_Timers_t;

typedef struct  {
	PCVo_Config_t* config;
	PCVo_Timers_t* timers;
	uint16_t Pulses_Per_Revolution;
	uint16_t oldNumPulses;
	uint32_t Measurement_Factor1;
} PCVo_object;

void PCVo_Init(PCVo_object* this,PCVo_Config_t* config,PCVo_Timers_t* timers);
void PCVo_Start(PCVo_object* this);
int32_t PCVo_CalculateVelocity(PCVo_object* this);

void Encoder_PeriodMeas_Update(int16_t *result);


#endif /* INC_PULSECOUNTVELOCITY_OBJECT_H_ */
