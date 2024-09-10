#pragma once
#ifndef _ADIS16480_H_
#define _ADIS16480_H_

#include "stm32f4xx_hal.h"
#include "spi.h"
#include <math.h>

/*
	@brief __USE_FULL_REGISTERS
	
	Uncomment this macro
	to enable full sensor register control

	Unlock full functions list
*/
#define __USE_FULL_REGISTERS

#define SPI_NOP 		0x00 // No operation. Use for dummy writes.
// User Register Memory Map from Table 9
#define PAGE_ID 		0x00 // 0x00, R/W, No, Page identifier, N/A
// Name, {PAGE_ID, Address} // Default, R/W, Flash, Register Description, Format
#define SEQ_CNT 		0x0006 // N/A, R, No, Sequence counter, Table 68
#define SYS_E_FLAG 		0x0008 // 0x0000, R, No, Output - system error flags, Table 59
#define DIAG_STS 		0x000A // 0x0000, R, No, Output - self-test error flags, Table 60
#define ALM_STS 		0x000C // 0x0000, R, No, Output - alarm error flags, Table 61
#define TEMP_OUT 		0x000E // N/A, R, No, Output - temperature, Table 57
#define X_GYRO_LOW 		0x0010 // N/A, R, No, Output - x-axis gyroscope low word, Table 14
#define X_GYRO_OUT 		0x0012 // N/A, R, No, Output - x-axis gyroscope high word, Table 10
#define Y_GYRO_LOW 		0x0014 // N/A, R, No, Output - y-axis gyroscope low word, Table 15
#define Y_GYRO_OUT 		0x0016 // N/A, R, No, Output - y-axis gyroscope high word, Table 11
#define Z_GYRO_LOW 		0x0018 // N/A, R, No, Output - z-axis gyroscope low word, Table 16
#define Z_GYRO_OUT 		0x001A // N/A, R, No, Output - z-axis gyroscope high word, Table 12
#define X_ACCL_LOW 		0x001C // N/A, R, No, Output - x-axis accelerometer low word, Table 21
#define X_ACCL_OUT 		0x001E // N/A, R, No, Output - x-axis accelerometer high word, Table 17
#define Y_ACCL_LOW 		0x0020 // N/A, R, No, Output - y-axis accelerometer low word, Table 22
#define Y_ACCL_OUT 		0x0022 // N/A, R, No, Output - y-axis accelerometer high word, Table 18
#define Z_ACCL_LOW 		0x0024 // N/A, R, No, Output - z-axis accelerometer low word, Table 23
#define Z_ACCL_OUT 		0x0026 // N/A, R, No, Output - z-axis accelerometer high word, Table 19
#define X_MAGN_OUT 		0x0028 // N/A, R, No, Output - x-axis magnetometer high word, Table 38
#define Y_MAGN_OUT 		0x002A // N/A, R, No, Output - y-axis magnetometer high word, Table 39
#define Z_MAGN_OUT 		0x002C // N/A, R, No, Output - z-axis magnetometer high word, Table 40
#define BAROM_LOW 		0x002E // N/A, R, No, Output - barometer low word, Table 56
#define BAROM_OUT 		0x0030 // N/A, R, No, Output - barometer high word, Table 54
#define X_DELTANG_LOW 	0x0040 // N/A, R, No, Output - x-axis delta angle low word, Table 28
#define X_DELTANG_OUT 	0x0042 // N/A, R, No, Output - x-axis delta angle high word, Table 24
#define Y_DELTANG_LOW 	0x0044 // N/A, R, No, Output - y-axis delta angle low word, Table 29
#define Y_DELTANG_OUT 	0x0046 // N/A, R, No, Output - y-axis delta angle high word, Table 25
#define Z_DELTANG_LOW 	0x0048 // N/A, R, No, Output - z-axis delta angle low word, Table 30
#define Z_DELTANG_OUT 	0x004A // N/A, R, No, Output - z-axis delta angle high word, Table 26
#define X_DELTVEL_LOW 	0x004C // N/A, R, No, Output - x-axis delta velocity low word, Table 35
#define X_DELTVEL_OUT 	0x004E // N/A, R, No, Output - x-axis delta velocity high word, Table 31
#define Y_DELTVEL_LOW 	0x0050 // N/A, R, No, Output - y-axis delta velocity low word, Table 36
#define Y_DELTVEL_OUT 	0x0052 // N/A, R, No, Output - y-axis delta velocity high word, Table 32
#define Z_DELTVEL_LOW 	0x0054 // N/A, R, No, Output - z-axis delta velocity low word, Table 37
#define Z_DELTVEL_OUT 	0x0056 // N/A, R, No, Output - z-axis delta velocity high word, Table 33
#define Q0_C11_OUT 		0x0060 // N/A, R/W, Yes, Quaternion q0 or rotation matrix C11, Table 42
#define Q1_C12_OUT 		0x0062 // N/A, R/W, Yes, Quaternion q1 or rotation matrix C12, Table 43
#define Q2_C13_OUT 		0x0064 // N/A, R/W, Yes, Quaternion q2 or rotation matrix C13, Table 44
#define Q3_C21_OUT 		0x0066 // N/A, R/W, Yes, Quaternion q3 or rotation matrix C21, Table 45
#define C22_OUT 		0x0068 // N/A, R/W, Yes, Rotation matrix C22, Table 46
#define ROLL_C23_OUT 	0x006A // N/A, R/W, Yes, Euler angle / roll axis / or rotation matrix C23, Table 47
#define PITCH_C31_OUT 	0x006C // N/A, R/W, Yes, Euler angle / pitch axis / or rotation matrix C31, Table 48
#define YAW_C32_OUT 	0x006E // N/A, R/W, Yes, Euler angle / yaw axis / or rotation matrix, C32, Table 49
#define C33_OUT 		0x0070 // N/A, R/W, Yes, Rotation matrix C33, Table 50
#define TIME_MS_OUT 	0x0078 // N/A, R, Yes, Factory configuration time: minutes/seconds, Table 156
#define TIME_DH_OUT 	0x007A // N/A, R, Yes, Factory configuration date/time: day/hour, Table 157
#define TIME_YM_OUT 	0x007C // N/A, R, Yes, Factory configuration date: year/month, Table 158
#define PROD_ID 		0x007E // 0x4060, R, Yes, Output, product identification (16,480), Table 65
#define X_GYRO_SCALE 	0x0204 // 0x0000, R/W, Yes, Calibration scale - x-axis gyroscope, Table 103
#define Y_GYRO_SCALE 	0x0206 // 0x0000, R/W, Yes, Calibration scale - y-axis gyroscope, Table 104
#define Z_GYRO_SCALE 	0x0208 // 0x0000, R/W, Yes, Calibration scale - z-axis gyroscope, Table 105
#define X_ACCL_SCALE 	0x020A // 0x0000, R/W, Yes, Calibration scale - x-axis accelerometer, Table 113
#define Y_ACCL_SCALE 	0x020C // 0x0000, R/W, Yes, Calibration scale - y-axis accelerometer, Table 114
#define Z_ACCL_SCALE 	0x020E // 0x0000, R/W, Yes, Calibration scale - z-axis accelerometer, Table 115
#define XG_BIAS_LOW 	0x0210 // 0x0000, R/W, Yes, Calibration offset - gyroscope x-axis low word, Table 100
#define XG_BIAS_HIGH 	0x0212 // 0x0000, R/W, Yes, Calibration offset - gyroscope x-axis high word, Table 97
#define YG_BIAS_LOW 	0x0214 // 0x0000, R/W, Yes, Calibration offset - gyroscope y-axis low word, Table 101
#define YG_BIAS_HIGH 	0x0216 // 0x0000, R/W, Yes, Calibration offset - gyroscope y-axis high word, Table 98
#define ZG_BIAS_LOW 	0x0218 // 0x0000, R/W, Yes, Calibration offset - gyroscope z-axis low word, Table 102
#define ZG_BIAS_HIGH 	0x021A // 0x0000, R/W, Yes, Calibration offset - gyroscope z-axis high word, Table 99
#define XA_BIAS_LOW 	0x021C // 0x0000, R/W, Yes, Calibration offset - accelerometer x-axis low word, Table 110
#define XA_BIAS_HIGH 	0x021E // 0x0000, R/W, Yes, Calibration offset - accelerometer x-axis high word, Table 107
#define YA_BIAS_LOW 	0x0220 // 0x0000, R/W, Yes, Calibration offset - accelerometer y-axis low word, Table 111
#define YA_BIAS_HIGH 	0x0222 // 0x0000, R/W, Yes, Calibration offset - accelerometer y-axis high word, Table 108
#define ZA_BIAS_LOW 	0x0224 // 0x0000, R/W, Yes, Calibration offset - accelerometer z-axis low word, Table 112
#define ZA_BIAS_HIGH 	0x0226 // 0x0000, R/W, Yes, Calibration offset - accelerometer z-axis high word, Table 109
#define HARD_IRON_X 	0x0228 // 0x0000, R/W, Yes, Calibration / hard iron - magnetometer x-axis, Table 116
#define HARD_IRON_Y 	0x022A // 0x0000, R/W, Yes, Calibration / hard iron - magnetometer y-axis, Table 117
#define HARD_IRON_Z 	0x022C // 0x0000, R/W, Yes, Calibration / hard iron - magnetometer z-axis, Table 118
#define SOFT_IRON_S11 	0x022E // 0x0000, R/W, Yes, Calibration / soft iron - magnetometer S11, Table 120
#define SOFT_IRON_S12 	0x0230 // 0x0000, R/W, Yes, Calibration / soft iron - magnetometer S12, Table 121
#define SOFT_IRON_S13 	0x0232 // 0x0000, R/W, Yes, Calibration / soft iron - magnetometer S13, Table 122
#define SOFT_IRON_S21 	0x0234 // 0x0000, R/W, Yes, Calibration / soft iron - magnetometer S21, Table 123
#define SOFT_IRON_S22 	0x0236 // 0x0000, R/W, Yes, Calibration / soft iron - magnetometer S22, Table 124
#define SOFT_IRON_S23 	0x0238 // 0x0000, R/W, Yes, Calibration / soft iron - magnetometer S23, Table 125
#define SOFT_IRON_S31 	0x023A // 0x0000, R/W, Yes, Calibration / soft iron - magnetometer S31, Table 126
#define SOFT_IRON_S32 	0x023C // 0x0000, R/W, Yes, Calibration / soft iron - magnetometer S32, Table 127
#define SOFT_IRON_S33 	0x023E // 0x0000, R/W, Yes, Calibration / soft iron - magnetometer S33, Table 128
#define BR_BIAS_LOW 	0x0240 // 0x0000, R/W, Yes, Calibration offset -, barometer low word, Table 131
#define BR_BIAS_HIGH 	0x0242 // 0x0000, R/W, Yes, Calibration offset -, barometer high word, Table 130
#define REFMTX_R11 		0x0262 // 0x7FFF, R/W, Yes, Reference transformation matrix R11, Table 84
#define REFMTX_R12 		0x0264 // 0x0000, R/W, Yes, Reference transformation matrix R12, Table 85
#define REFMTX_R13 		0x0266 // 0x0000, R/W, Yes, Reference transformation matrix R13, Table 86
#define REFMTX_R21 		0x0268 // 0x0000, R/W, Yes, Reference transformation matrix R21, Table 87
#define REFMTX_R22 		0x026A // 0x7FFF, R/W, Yes, Reference transformation matrix R22, Table 88
#define REFMTX_R23 		0x026C // 0x0000, R/W, Yes, Reference transformation matrix R23, Table 89
#define REFMTX_R31 		0x026E // 0x0000, R/W, Yes, Reference transformation matrix R31, Table 90
#define REFMTX_R32 		0x0270 // 0x0000, R/W, Yes, Reference transformation matrix R32, Table 91
#define REFMTX_R33 		0x0272 // 0x7FFF, R/W, Yes, Reference transformation matrix R33, Table 92
#define USER_SCR_1 		0x0274 // 0x0000, R/W, Yes, User Scratch Register 1, Table 152
#define USER_SCR_2 		0x0276 // 0x0000, R/W, Yes, User Scratch Register 2, Table 153
#define USER_SCR_3 		0x0278 // 0x0000, R/W, Yes, User Scratch Register 3, Table 154
#define USER_SCR_4 		0x027A // 0x0000, R/W, Yes, User Scratch Register 4, Table 155
#define FLSHCNT_LOW 	0x027C // N/A, R, Yes, Diagnostic - flash memory count low word, Table 147
#define FLSHCNT_HIGH 	0x027E // N/A, R, Yes, Diagnostic - flash memory count high word, Table 148
#define GLOB_CMD 		0x0302 // N/A, W, No, Control - global commands, Table 146
#define FNCTIO_CTRL 	0x0306 // 0x000D, R/W, Yes, Control - I/O pins functional definitions, Table 149
#define GPIO_CTRL 		0x0308 // 0x00X01, R/W, Yes, Control - I/O pins general purpose, Table 150
#define CONFIG			0x030A // 0x00C0, R/W, Yes, Control - clock and miscellaneous correction, Table 106
#define DEC_RATE 		0x030C // 0x0000, R/W, Yes, Control - output sample rate decimation, Table 67
#define SLP_CNT 		0x0310 // N/A, R/W, No, Control - power-down/sleep mode, Table 151
#define FILTR_BNK_0 	0x0316 // 0x0000, R/W, Yes, Filter selection, Table 69
#define FILTR_BNK_1 	0x0318 // 0x0000, R/W, Yes, Filter selection, Table 70
#define ALM_CNFG_0		0x0320 // 0x0000, R/W, Yes, Alarm configuration, Table 142
#define ALM_CNFG_1 		0x0322 // 0x0000, R/W, Yes, Alarm configuration, Table 143
#define ALM_CNFG_2 		0x0324 // 0x0000, R/W, Yes, Alarm configuration, Table 144
#define XG_ALM_MAGN 	0x0328 // 0x0000, R/W, Yes, Alarm - x-axis gyroscope threshold setting, Table 132
#define YG_ALM_MAGN 	0x032A // 0x0000, R/W, Yes, Alarm - y-axis gyroscope threshold setting, Table 133
#define ZG_ALM_MAGN 	0x032C // 0x0000, R/W, Yes, Alarm - z-axis gyroscope threshold setting, Table 134
#define XA_ALM_MAGN 	0x032E // 0x0000, R/W, Yes, Alarm - x-axis accelerometer threshold, Table 135
#define YA_ALM_MAGN 	0x0330 // 0x0000, R/W, Yes, Alarm - y-axis accelerometer threshold, Table 136
#define ZA_ALM_MAGN 	0x0332 // 0x0000, R/W, Yes, Alarm - z-axis accelerometer threshold, Table 137
#define XM_ALM_MAGN 	0x0334 // 0x0000, R/W, Yes, Alarm - x-axis magnetometer threshold, Table 138
#define YM_ALM_MAGN 	0x0336 // 0x0000, R/W, Yes, Alarm - y-axis magnetometer threshold, Table 139
#define ZM_ALM_MAGN 	0x0338 // 0x0000, R/W, Yes, Alarm - z-axis magnetometer threshold, Table 140
#define BR_ALM_MAGN 	0x033A // 0x0000, R/W, Yes, Alarm - barometer threshold setting, Table 141
#define EKF_CNFG 		0x0350 // 0x0200, R/W, Yes, Extended Kalman filter configuration, Table 94
#define DECLN_ANGL 		0x0354 // 0x0000, R/W, Yes, Declination angle, Table 93
#define ACC_DISTB_THR 	0x0356 // 0x0020, R/W, Yes, Accelerometer disturbance threshold, Table 95
#define MAG_DISTB_THR 	0x0358 // 0x0030, R/W, Yes, Magnetometer disturbance threshold, Table 96
#define QCVR_NOIS_LWR 	0x0360 // 0xC5AC, R/W, Yes, Process covariance - gyroscope noise lower word, Table 77
#define QCVR_NOIS_UPR 	0x0362 // 0x3727, R/W, Yes, Process covariance - gyroscope noise upper word, Table 76
#define QCVR_RRW_LWR 	0x0364 // 0xE6FF, R/W, Yes, Process covariance - gyroscope RRW lower word, Table 79
#define QCVR_RRW_UPR 	0x0366 // 0x2E5B, R/W, Yes, Process covariance - gyroscope RRW upper word, Table 78
#define RCVR_ACC_LWR 	0x036C // 0x705F, R/W, Yes, Measurement covariance - accelerometer upper, Table 81
#define RCVR_ACC_UPR 	0x036E // 0x3189, R/W, Yes, Measurement covariance - accelerometer lower, Table 80
#define RCVR_MAG_LWR 	0x0370 // 0xCC77, R/W, Yes, Measurement covariance - magnetometer upper, Table 83
#define RCVR_MAG_UPR 	0x0372 // 0x32AB, R/W, Yes, Measurement covariance - magnetometer lower, Table 82
#define FIRM_REV 		0x0378 // N/A, R, Yes, Firmware revision, Table 62
#define FIRM_DM 		0x037A // N/A, R, Yes, Firmware programming date: day/month, Table 63
#define FIRM_Y 			0x037C // N/A, R, Yes, Firmware programming date: year, Table 64
#define SERIAL_NUM 		0x0420 // N/A, R, Yes, Serial number, Table 66
#define FIR_COEF_A_LOW 	0x0502 // to 0x7E N/A, R/W, Yes, FIR Filter Bank A Coefficients 0 through 59, Table 71
#define FIR_COEF_A_HIGH 0x0602 // to 0x7E N/A, R/W, Yes, FIR Filter Bank A Coefficients 60 through 119, Table 71
#define FIR_COEF_B_LOW 	0x0702 // to 0x7E N/A, R/W, Yes, FIR Filter Bank B Coefficients 0 through 59, Table 72
#define FIR_COEF_B_HIGH 0x0802 // to 0x7E N/A, R/W, Yes, FIR Filter Bank B Coefficients 60 through 119, Table 72
#define FIR_COEF_C_LOW 	0x0902 // to 0x7E N/A, R/W, Yes, FIR Filter Bank C Coefficients 0 through 59, Table 73
#define FIR_COEF_C_HIGH 0x0A02 // to 0x7E N/A, R/W, Yes, FIR Filter Bank C Coefficients 60 through 119, Table 73
#define FIR_COEF_D_LOW 	0x0B02 // to 0x7E N/A, R/W, Yes, FIR Filter Bank D Coefficients 0 through 59, Table 74
#define FIR_COEF_D_HIGH 0x0C02 // to 0x7E N/A, R/W, Yes, FIR Filter Bank D Coefficients 60 through 119, Table 74

typedef struct {
	/* Interfaces */

    SPI_HandleTypeDef *interface;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;

	/* Main ADIS output parameters */
	float euler_angle[3];
	float euler_scale_var;

	float linear_acceleration[3];
	float accl_scale_var;

	float angular_velocity[3];
	float gyro_scale_var;

	float magnetic_field[3];
	float magn_course;
	float magn_scale_var;

	float temperature;

	float pressure;
	float pressure_scale_var;
	/* Raw Registers */

	unsigned int seq_cnt : 6;
	/*
		[SYS_E_FLAG]
		Page 0, Base address = 0x08
	*/
	union {
		uint16_t 			adis_register;
		struct {
			unsigned int 	watchdog_flag : 1;
			unsigned int	not_used_0 : 1;
			unsigned int 	ekf_divergence : 1;
			unsigned int 	gyroscope_saturation : 1;
			unsigned int 	magnetometer_disturbance : 1;
			unsigned int 	acceleration_threshold_flag : 1;
			unsigned int	barometer_new_data : 1;
			unsigned int	magnetometer_new_data : 1;
			unsigned int	processing_overrun : 1;
			unsigned int	flash_memory_update_flag : 1;
			unsigned int	inertial_self_test_flag : 1;
			unsigned int	sensor_overrange : 1;
			unsigned int	spi_connection_error : 1;
			unsigned int	not_used_1 : 2;
			unsigned int	alarm_status_flag : 1;
		} bits;
	} sys_e_flag;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int 	barometer_self_test_failure : 1;
			unsigned int 	magn_z_self_test_failure : 1;
			unsigned int 	magn_y_self_test_failure : 1;
			unsigned int 	magn_x_self_test_failure : 1;
			unsigned int 	accel_z_self_test_failure : 1;
			unsigned int 	accel_y_self_test_failure : 1;
			unsigned int 	accel_x_self_test_failure : 1;
			unsigned int 	gyro_z_self_test_failure : 1;
			unsigned int 	gyro_y_self_test_failure : 1;
			unsigned int 	gyro_x_self_test_failure : 1;
		} bits;
	} diag_sts;

	union {
		uint16_t			adis_register;
		struct {
			unsigned int 	barometer_alarm_flag : 1;
			unsigned int 	magn_z_alarm_flag : 1;
			unsigned int 	magn_y_alarm_flag : 1;
			unsigned int 	magn_x_alarm_flag : 1;
			unsigned int 	accel_z_alarm_flag : 1;
			unsigned int 	accel_y_alarm_flag : 1;
			unsigned int 	accel_x_alarm_flag : 1;
			unsigned int 	gyro_z_alarm_flag : 1;
			unsigned int 	gyro_y_alarm_flag : 1;
			unsigned int 	gyro_x_alarm_flag : 1;	
		} bits;
	} alm_sts;

	int16_t 				temp_out;
	int16_t 				x_gyro_low;
	int16_t 				x_gyro_out;
	int16_t 				y_gyro_low;
	int16_t 				y_gyro_out;
	int16_t 				z_gyro_low;
	int16_t 				z_gyro_out;
	int16_t 				x_accl_low;
	int16_t 				x_accl_out;
	int16_t 				y_accl_low;
	int16_t 				y_accl_out;
	int16_t 				z_accl_low;
	int16_t 				z_accl_out;
	int16_t 				x_magn_out;
	int16_t 				y_magn_out;
	int16_t 				z_magn_out;
	int16_t 				barom_low;
	int16_t 				barom_out;
	int16_t 				x_deltang_low;
	int16_t 				x_deltang_out;
	int16_t 				y_deltang_low;
	int16_t 				y_deltang_out;
	int16_t 				z_deltang_low;
	int16_t 				z_deltang_out;	
	int16_t 				x_deltvel_low;
	int16_t 				x_deltvel_out;
	int16_t 				y_deltvel_low;
	int16_t 				y_deltvel_out;
	int16_t 				z_deltvel_low;
	int16_t 				z_deltvel_out;	
	int16_t 				q0_c11_out;
	int16_t 				q1_c12_out;
	int16_t 				q2_c13_out;
	int16_t 				q3_c21_out;
	int16_t 				c22_out;
	int16_t 				roll_c23_out;
	int16_t 				pitch_c31_out;
	int16_t 				yaw_c32_out;
	int16_t 				c33_out;

#ifdef __USE_FULL_REGISTERS
	union {
		uint16_t			adis_register;
		struct {
			unsigned int 	not_used_0 : 2;
			unsigned int 	minutes : 6;
			unsigned int 	not_used_1 : 2;
			unsigned int 	seconds : 6;			
		} bits;
	} time_ms_out;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int	not_used_0 : 3;
			unsigned int 	day : 5;
			unsigned int	not_used_1 : 2;
			unsigned int 	hours : 6;
		} bits;

	} time_dh_out;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int	not_used_0 : 1;
			unsigned int 	year : 7;
			unsigned int 	not_used_1 : 4;
			unsigned int 	month : 4;
		} bits;
	} time_ym_out;
#endif /*__USE_FULL_REGISTERS*/

	uint16_t 				prod_id;
	uint16_t 				x_gyro_scale;
	uint16_t 				y_gyro_scale;
	uint16_t 				z_gyro_scale;
	uint16_t 				x_accl_scale;
	uint16_t 				y_accl_scale;
	uint16_t 				z_accl_scale;	

#ifdef __USE_FULL_REGISTERS
	uint16_t 				xg_bias_low;
	uint16_t 				xg_bias_high;
	uint16_t 				yg_bias_low;
	uint16_t 				yg_bias_high;
	uint16_t 				zg_bias_low;
	uint16_t 				zg_bias_high;		
	uint16_t 				xa_bias_low;
	uint16_t 				xa_bias_high;
	uint16_t 				ya_bias_low;
	uint16_t 				ya_bias_high;
	uint16_t 				za_bias_low;
	uint16_t 				za_bias_high;	
	uint16_t 				hard_iron_x;
	uint16_t 				hard_iron_y;
	uint16_t 				hard_iron_z;
	uint16_t 				soft_iron_s11;
	uint16_t 				soft_iron_s12;
	uint16_t 				soft_iron_s13;
	uint16_t 				soft_iron_s21;
	uint16_t 				soft_iron_s22;
	uint16_t 				soft_iron_s23;
	uint16_t 				soft_iron_s31;
	uint16_t 				soft_iron_s32;
	uint16_t 				soft_iron_s33;
	uint16_t 				br_bias_low;
	uint16_t 				br_bias_high;
	uint16_t 				refmtx_r11;
	uint16_t 				refmtx_r12;
	uint16_t 				refmtx_r13;
	uint16_t 				refmtx_r21;
	uint16_t 				refmtx_r22;
	uint16_t 				refmtx_r23;
	uint16_t 				refmtx_r31;
	uint16_t 				refmtx_r32;
	uint16_t 				refmtx_r33;
	uint16_t 				user_scr_1;
	uint16_t 				user_scr_2;
	uint16_t 				user_scr_3;
	uint16_t 				user_scr_4;
	uint16_t 				flshcnt_low;
	uint16_t 				flshcnt_high;
#endif /*__USE_FULL_REGISTERS*/

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int 	ekf_reset : 1;
			unsigned int	not_used_0 : 5;
			unsigned int 	reset_ref_rotation_matrix : 1;
			unsigned int 	tare_command : 1;
			unsigned int 	soft_reset : 1;
			unsigned int 	factory_calibration_restore : 1;
			unsigned int	not_used_1 : 2;
			unsigned int 	flash_mem_update : 1;
			unsigned int 	flash_mem_test : 1;
			unsigned int 	self_test : 1;
			unsigned int	not_used_2 : 1;
		} bits;
	} glob_cmd;

	union {
		uint16_t			adis_register;
		struct {
			unsigned int	not_used_0 : 4;
			unsigned int 	alarm_indicator : 1;
			unsigned int 	alarm_indicator_pol : 1;
			unsigned int 	alarm_indicator_line : 2;
			unsigned int 	sync_clock_in_en : 1;
			unsigned int 	sync_clock_in_pol : 1;
			unsigned int 	sync_clock_in_line : 2;
			unsigned int 	data_ready_en : 1;
			unsigned int 	data_ready_pol : 1;
			unsigned int 	data_ready_line : 2;
		} bits;
	} fnctio_ctrl;

	union {
		uint16_t			adis_register;
		struct {
			unsigned int	not_used_0 : 8;
			unsigned int 	dio4_level : 1;
			unsigned int 	dio3_level : 1;
			unsigned int 	dio2_level : 1;
			unsigned int 	dio1_level : 1;
			unsigned int 	dio4_direction_ctrl : 1;
			unsigned int 	dio3_direction_ctrl : 1;
			unsigned int 	dio2_direction_ctrl : 1;
			unsigned int 	dio1_direction_ctrl : 1;
		} bits;
	} gpio_ctrl;

	union {
		uint16_t			adis_register;
		struct {
			unsigned int	not_used_0 : 8;
			unsigned int 	g_compensation_f_gyro : 1;
			unsigned int 	p_percussion_allign : 1;
			unsigned int	not_used_1 : 4;
			unsigned int 	rtc_saving_time : 1;
			unsigned int 	rtc_clock_ctrl : 1;
		} bits;
	} config;

	unsigned int 			dec_rate : 10;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int 	not_used_0 : 6;
			unsigned int 	power_down_mode : 1;
			unsigned int 	normal_sleep_mode : 1;
			unsigned int 	programmable_time_bits : 8;			
		} bits;
	} slp_cnt;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int	not_used_0 : 1;
			unsigned int 	y_accl_filter_en : 1;
			unsigned int 	y_accl_filter_select : 2;
			unsigned int 	x_accl_filter_en : 1;
			unsigned int 	x_accl_filter_select : 2;
			unsigned int 	z_gyro_filter_en : 1;
			unsigned int 	z_gyro_filter_select : 2;
			unsigned int 	y_gyro_filter_en : 1;
			unsigned int 	y_gyro_filter_select : 2;
			unsigned int 	x_gyro_filter_en : 1;
			unsigned int 	x_gyro_filter_select : 2;
		} bits;
	} fltr_bnk_0;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int 	not_used_0 : 4;
			unsigned int 	z_magn_filter_en : 1;
			unsigned int 	z_magn_filter_select : 2;
			unsigned int 	y_magn_filter_en : 1;
			unsigned int 	y_magn_filter_select : 2;
			unsigned int 	x_magn_filter_en : 1;
			unsigned int 	x_magn_filter_select : 2;
			unsigned int 	z_accl_filter_en : 1;
			unsigned int 	z_accl_filter_select : 2;
		} bits;
	} fltr_bnk_1;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int 	x_accl_alarm : 1;
			unsigned int 	not_used_0 : 1;
			unsigned int 	x_accl_alarm_pol : 1;
			unsigned int 	x_accl_dynamic_en : 1;
			unsigned int 	z_gyro_alarm : 1;
			unsigned int 	not_used_1 : 1;
			unsigned int 	z_gyro_alarm_pol : 1;
			unsigned int 	z_gyro_dynamic_en : 1;
			unsigned int 	y_gyro_alarm : 1;
			unsigned int 	not_used_2 : 1;
			unsigned int 	y_gyro_alarm_pol : 1;
			unsigned int 	y_gyro_dynamic_en : 1;
			unsigned int 	x_gyro_alarm : 1;
			unsigned int 	not_used_3 : 1;
			unsigned int 	x_gyro_alarm_pol : 1;
			unsigned int 	x_gyro_dynamic_en : 1;
		} bits;
	} alm_cnfg_0;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int 	y_magn_alarm : 1;
			unsigned int 	not_used_0 : 1;
			unsigned int 	y_magn_alarm_pol : 1;
			unsigned int 	y_magn_dynamic_en : 1;
			unsigned int 	x_magn_alarm : 1;
			unsigned int 	not_used_1 : 1;
			unsigned int 	x_magn_alarm_pol : 1;
			unsigned int 	x_magn_dynamic_en : 1;
			unsigned int 	z_accl_alarm : 1;
			unsigned int 	not_used_2 : 1;
			unsigned int 	z_accl_alarm_pol : 1;
			unsigned int 	z_accl_dynamic_en : 1;
			unsigned int 	y_accl_alarm : 1;
			unsigned int 	not_used_3 : 1;
			unsigned int 	y_accl_alarm_pol : 1;
			unsigned int 	y_accl_dynamic_en : 1;
		} bits;
	} alm_cnfg_1;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int	not_used_0 : 8;
			unsigned int 	barometer_alarm : 1;
			unsigned int	not_used_1 : 1;
			unsigned int 	barometer_alarm_pol : 1;
			unsigned int 	barometer_dynamic_en : 1;
			unsigned int 	z_magn_alarm : 1;
			unsigned int	not_used_2 : 1;
			unsigned int 	z_magn_alarm_pol : 1;
			unsigned int 	z_magn_dynamic_en : 1;
		} bits;
	} alm_cnfg_2;

	uint16_t 				xg_alm_magn;
	uint16_t 				yg_alm_magn;
	uint16_t 				zg_alm_magn;
	uint16_t 				xa_alm_magn;
	uint16_t 				ya_alm_magn;
	uint16_t 				xm_alm_magn;
	uint16_t 				ym_alm_magn;
	uint16_t 				zm_alm_magn;
	uint16_t 				br_alm_magn;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int	not_used_0 : 3;
			unsigned int 	auto_reset_recovery : 1;
			unsigned int	not_used_1 : 2;
			unsigned int 	fade_en : 1;
			unsigned int 	adaptive_ekf_en : 1;
			unsigned int	not_used_2 : 2;
			unsigned int 	orientation_format : 1;
			unsigned int 	body_frame_select : 1;
			unsigned int 	magn_disable : 1;
			unsigned int 	graviry_removal : 1;
		} bits;
	} ekf_cnfg;

	uint16_t 				decln_angl;
	uint16_t 				acc_distb_thr;
	uint16_t 				mag_distb_thr;
	uint16_t 				qcvr_nois_lwr;
	uint16_t 				qcvr_nois_upr;
	uint16_t 				qcvr_rrw_lwr;
	uint16_t 				qcvr_rrw_upr;
	uint16_t 				rcvr_acc_lwr;
	uint16_t 				rcvr_acc_upr;
	uint16_t 				rcvr_mag_lwr;
	uint16_t 				rcvr_mag_upr;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int binary_rev_10s_d : 4;
			unsigned int binary_rev_1s_d : 4;
			unsigned int binary_rev_tenths_d : 4;
			unsigned int binary_rev_hundredths_d : 4;
		} bits;
	} firm_rev;

	union {
		uint16_t			adis_register;
		struct {
			unsigned int 	binary_month_10s_d : 4;
			unsigned int 	binary_month_1s_d : 4;
			unsigned int 	binary_day_10s_d : 4;
			unsigned int 	binary_day_1s_d : 4;	
		} bits;
	} firm_dm;

	union {
		uint16_t 			adis_register;
		struct {
			unsigned int 	binary_year_1000s_d : 4;
			unsigned int 	binary_year_100s_d : 4;
			unsigned int 	binary_year_10s_d : 4;
			unsigned int 	binary_year_1s_d : 4;
		} bits;
	} firm_y;

	uint16_t 				serial_num;
	// future todo work
#ifdef TODO
	uint16_t fir_coef_a[120];
	uint16_t fir_coef_b[120];
	uint16_t fir_coef_c[120];
	uint16_t fir_coef_d[120];
#endif
}adis16480_t;

uint8_t adis16480_init(adis16480_t *object,
                    SPI_HandleTypeDef *interface, 
                    GPIO_TypeDef *port,
                    uint16_t pin);

void adis16480_tick(adis16480_t *sensor);

void adis16480_reset(adis16480_t *object);

void adis16480_set_body_frame(adis16480_t *sensor);

void adis16480_update_euler_angles(adis16480_t *sensor);

void adis16480_set_gyro_scale_to_rads(adis16480_t *sensor);

void adis16480_set_gyro_scale_to_degs(adis16480_t *sensor);

void adis16480_set_gyro_scale_custom(adis16480_t *sensor, float new_scale);

void adis16480_set_accl_scale(adis16480_t *sensor, float new_scale);

void adis16480_set_magn_scale_to_tesla(adis16480_t *sensor);

void adis16480_set_magn_scale_to_gauss(adis16480_t *sensor);

void adis16480_set_magn_scale_custom(adis16480_t *sensor, float new_scale);

void adis16480_update_acceleration(adis16480_t *object);

void adis16480_update_angular_velocity(adis16480_t *object);

void adis16480_update_magnetic_course(adis16480_t *object);

void adis16480_read_seq_cnt(adis16480_t *object);

void adis16480_read_sys_e_flag(adis16480_t *object);

void adis16480_tare(adis16480_t *object);

void adis16480_self_test(adis16480_t *object);

void adis16480_set_body_frame(adis16480_t *object);

void adis16480_set_dec_rate(uint16_t value);

void adis16480_set_ref_matrix(int16_t *matrix);

#endif /*_ADIS16480_H_*/
