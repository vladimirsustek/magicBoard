/*
 * measurements.h
 *
 *  Created on: Mar 3, 2023
 *      Author: 42077
 */

#ifndef MEASUREMENTS_H_
#define MEASUREMENTS_H_

#include <stdbool.h>
#include <stdint.h>

void measurements_open();
void measurement_critical_enter();
void measurement_critical_exit();
bool is_measurement_critical();
void measurement_close();
bool measurement_get(int32_t* ch12, int32_t* temp);

#endif /* MEASUREMENTS_H_ */
