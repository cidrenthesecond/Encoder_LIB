/*
 * encoder.h
 *
 *  Created on: Feb 24, 2026
 *      Author: 1
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

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
	int16_t pulses;
	int16_t old_pulses;
	int callbacks_elapsed;
	int result;
	const int constat;
} Encoder_PulseMeas_Ctx_t;

typedef struct {
	uint8_t encoder_poles;
	uint8_t encoder_edges_counted;
	uint8_t encoder_channels_counted;
	int measurement_frequency;
} Encoder_PulseMeas_Config_t;

typedef enum {
	OK = 1,
	NO_PULSE
} Encoder_Status_t;

Encoder_Status_t Encoder_PulseMeas_Update(int16_t* result);
Encoder_Status_t M_Method_Init(Encoder_PulseMeas_Ctx_t* ctx);
Encoder_Status_t Encoder_PulseMeas_Update1(Encoder_PulseMeas_Ctx_t* ctx);

Encoder_Status_t Encoder_PeroidMeas_Update(int16_t *result);

#endif /* INC_ENCODER_H_ */
