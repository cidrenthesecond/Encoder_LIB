/*
 * encoder.h
 *
 *  Created on: Feb 24, 2026
 *      Author: 1
 */

#ifndef INC_PULSEMEAS_H_
#define INC_PULSEMEAS_H_

#include "main.h"

#define Encoder_Poles               3
#define Encoder_Edges_Counted       2
#define Encoder_Channels_Counted    1
#define Encoder_PulsesPerRevolution (Encoder_Poles*Encoder_Edges_Counted*Encoder_Channels_Counted)

//metoda impulsow w czasie
#define Measurement_Frequency       1 //in hertz
#define Measurement_Factor          60*Measurement_Frequency/Encoder_PulsesPerRevolution

/*

for this method to work you need:

*Timer configured as clock to generate periodical interrupts
*Timer configured in encoder mode to count impulses genereted
 in given time

*/
typedef struct {
	TIM_TypeDef *Tim_Used;
	uint32_t Measurement_Frequency1;
	uint8_t Encoder_Poles1;
	uint8_t Encoder_Edges_Counted1;
	uint8_t Encoder_Channels_Counted1;
}Encoder_Info_t;

struct Encoder_PulseMeas {
	Encoder_Info_t Encoder_Info;
	uint8_t Pulses_Per_Revolution;
	uint8_t oldNumPulses;
	uint32_t Measurement_Factor1;

	int16_t (*CalculateVelocity)(struct Encoder_PulseMeas* this);
};

uint8_t Calculate_pulsesPerRevolution(Encoder_Info_t* info);
uint32_t Calculate_measurementFactor(Encoder_Info_t* info);

void Encoder_PulseMeas_Update(int16_t* result);

void Encoder_PeroidMeas_Update(int16_t *result);

#endif /* INC_PULSEMEAS_H_ */
