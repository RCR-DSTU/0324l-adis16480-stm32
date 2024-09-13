#pragma once
#ifndef _ADIS16480_H_
#define _ADIS16480_H_

#if defined(STM32F401xx) || defined(STM32F407xx) || defined(STM32F411xx) ||\
	defined(STM32F427xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32F103xx)
#include "stm32f1xx.hal.h"
#elif defined(STM32F745xx)
#include "stm32f7xx.hal.h"
#else
#error "Missing required STM32 chip define. Check global define settings"
#error "Check and accept that your main.h file have include base peripheral lib"
#define ERROR_RECEIVED
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
	@brief __ADIS16480_FULL_REGS
	
	Uncomment this macro
	to enable full sensor register control

	Unlock full functions list
*/
//#define __ADIS16480_FULL_REGS

#ifndef ERROR_RECEIVED

/*
	Encapsulation expression
		__adis16480_t - private struct
*/
struct __adis16480_t;
typedef struct __adis16480_t adis16480_t;

adis16480_t *adis16480_init(SPI_HandleTypeDef *interface, 
                            GPIO_TypeDef *cs_port,
                            uint16_t cs_pin);

void adis16480_calibration_end(adis16480_t *sensor);

void adis16480_tick(adis16480_t *);

void adis16480_reset(adis16480_t *);

void adis16480_set_body_frame(adis16480_t *);

void adis16480_update_euler_angles(adis16480_t *);

void adis16480_set_gyro_scale_to_rads(adis16480_t *);

void adis16480_set_gyro_scale_to_degs(adis16480_t *);

void adis16480_set_gyro_scale_custom(adis16480_t *, float);

void adis16480_set_accl_scale(adis16480_t *, float);

void adis16480_set_magn_scale_to_tesla(adis16480_t *);

void adis16480_set_magn_scale_to_gauss(adis16480_t *);

void adis16480_set_magn_scale_custom(adis16480_t *, float);

void adis16480_update_acceleration(adis16480_t *);

void adis16480_update_angular_velocity(adis16480_t *);

void adis16480_update_magnetic_course(adis16480_t *);

void adis16480_read_seq_cnt(adis16480_t *);

void adis16480_read_sys_e_flag(adis16480_t *);

void adis16480_read_diag_sts(adis16480_t *);

void adis16480_tare(adis16480_t *);

void adis16480_self_test(adis16480_t *);

void adis16480_set_body_frame(adis16480_t *);

void adis16480_set_dec_rate(uint16_t);

void adis16480_set_ref_matrix(int16_t *);
#endif /*ERROR_RECEIVED*/
#endif /*_ADIS16480_H_*/
