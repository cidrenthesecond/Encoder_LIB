/*
 * PeriodMeas.h
 *
 *  Created on: Mar 5, 2026
 *      Author: 1
 */

#ifndef INC_TIMEINTERVALVELOCITY_STATIC_H_
#define INC_TIMEINTERVALVELOCITY_STATIC_H_

#include "main.h"

typedef enum {
	TIV_CHANNEL_A,
	TIV_CHANNEL_B
}TIV_Channel_t;

int32_t TIVs_CalculateVelocity(TIV_Channel_t channel);
void TIVs_Start();

#endif /* INC_TIMEINTERVALVELOCITY_STATIC_H_ */
