/*
 * ultrasonic.h
 *
 *  Created on: Mar 24, 2019
 *      Author: desta
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <msp430.h>
#include <inttypes.h>

#include <stdio.h>


uint16_t        time;
uint16_t        start;
float       distance;
uint16_t        current_state;
uint16_t        edge;

void trigger();
void getData();

#endif /* SENSOR_H_ */
