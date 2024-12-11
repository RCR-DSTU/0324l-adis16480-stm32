#include "adis16480.h"
#ifndef ERROR_RECEIVED
struct __adis16480_t {
	/* Interfaces */

    SPI_HandleTypeDef *interface;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    unsigned int is_dma_transfer : 1;

	/* Main ADIS output parameters */
	float euler_angle[3];
	int8_t euler_angle_dirs[3];
	float euler_scale_var;

	float linear_acceleration[3];
	int8_t accelerometer_dirs[3];
	float accl_scale_var;

	float angular_velocity[3];
	int8_t gyroscope_dirs[3];
	float gyro_scale_var;

	float magnetic_field[3];
	int8_t magnetic_field_dirs[3];
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

#ifdef __ADIS16480_FULL_REGS
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
#endif /*__ADIS16480_FULL_REGS*/

	uint16_t 				prod_id;
	uint16_t 				x_gyro_scale;
	uint16_t 				y_gyro_scale;
	uint16_t 				z_gyro_scale;
	uint16_t 				x_accl_scale;
	uint16_t 				y_accl_scale;
	uint16_t 				z_accl_scale;	

#ifdef __ADIS16480_FULL_REGS
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
#endif /*__ADIS16480_FULL_REGS*/

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
#ifdef __ADIS16480_FULL_REGS
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
#endif /*TODO*/
#endif /*__ADIS16480_FULL_REGS*/
};

static HAL_StatusTypeDef adis16480_write_register(adis16480_t *sensor, uint16_t reg_addr, uint16_t value);
static uint16_t adis16480_read_register(adis16480_t *sensor, uint16_t reg_addr);

adis16480_t *adis16480_init(SPI_HandleTypeDef *interface, 
                            GPIO_TypeDef *cs_port,
                            uint16_t cs_pin)
{
    adis16480_t *sensor;
    sensor = malloc(sizeof(adis16480_t));
    if(sensor == NULL) return NULL;
    memset(sensor, 0x00, sizeof(adis16480_t));

    sensor->interface = interface;
    sensor->cs_port = cs_port;
    sensor->cs_pin = cs_pin;

    sensor->prod_id = adis16480_read_register(sensor, PROD_ID);
    sensor->dec_rate = adis16480_read_register(sensor, DEC_RATE);

    // initial scaling to m/s^2
    sensor->accl_scale_var = 0.00784f;
    // initial scaling to deg/s
    sensor->gyro_scale_var = 0.02f;
    // initial scaling to gauss
    sensor->magn_scale_var = 0.0001f;
    // initial scaling to degrees
    sensor->euler_scale_var = 0.00549324f;

    // default axes directions
    memset(&sensor->accelerometer_dirs[0], 0x01, sizeof(sensor->accelerometer_dirs));
    memset(&sensor->gyroscope_dirs[0], 0x01, sizeof(sensor->gyroscope_dirs));
    memset(&sensor->magnetic_field_dirs[0], 0x01, sizeof(sensor->magnetic_field_dirs));

    return sensor;
}

void adis16480_calibration_end(adis16480_t *sensor)
{
    adis16480_set_body_frame(sensor);
    adis16480_tare(sensor);
}

/*
    Standard ADIS processing function

    @brief __weak adis16480_tick
        @param asis16480_t *sensor - sensor pointer typedef
    @retval None
*/
__weak void adis16480_tick(adis16480_t *sensor)
{
    adis16480_update_acceleration(sensor);
    adis16480_update_angular_velocity(sensor);
    adis16480_update_magnetic_course(sensor);
    adis16480_update_euler_angles(sensor);
}

__weak void adis16480_diagnostic_tick(adis16480_t *sensor)
{
    adis16480_read_diag_sts(sensor);
    adis16480_read_sys_e_flag(sensor);
}

void adis16480_reset(adis16480_t *sensor)
{
    adis16480_write_register(sensor, GLOB_CMD, 0x80);
    HAL_Delay(1800);
}

void adis16480_set_body_frame(adis16480_t *sensor)
{
    adis16480_write_register(sensor, EKF_CNFG, 0x8);
}

/*
    Gyro operations
*/

/*
    @brief adis16480_update_angular_velocity
*/
void adis16480_update_angular_velocity(adis16480_t *sensor)
{
	sensor->x_gyro_low = adis16480_read_register(sensor, X_GYRO_LOW);
    sensor->x_gyro_out = adis16480_read_register(sensor, X_GYRO_OUT);
    sensor->y_gyro_low = adis16480_read_register(sensor, Y_GYRO_LOW);
    sensor->y_gyro_out = adis16480_read_register(sensor, Y_GYRO_OUT);
    sensor->z_gyro_low = adis16480_read_register(sensor, Z_GYRO_LOW);
    sensor->z_gyro_out = adis16480_read_register(sensor, Z_GYRO_OUT);

    if(sensor->dec_rate == 0x00)
    {
        sensor->angular_velocity[0] = 
            (int32_t)sensor->x_gyro_out * sensor->gyro_scale_var *
            sensor->gyroscope_dirs[0];
        sensor->angular_velocity[1] = 
            (int32_t)sensor->y_gyro_out * sensor->gyro_scale_var *
            sensor->gyroscope_dirs[1];
        sensor->angular_velocity[2] = 
            (int32_t)sensor->z_gyro_out * sensor->gyro_scale_var *
            sensor->gyroscope_dirs[2];
    }
}

/*
    @brief adis16480_set_gyro_scale_to_rads
*/
void adis16480_set_gyro_scale_to_rads(adis16480_t *sensor)
{
    sensor->gyro_scale_var = (0.02f * M_PI / 180.0f);
}

void adis16480_set_gyro_scale_to_degs(adis16480_t *sensor)
{
    sensor->gyro_scale_var = 0.02f;
}

void adis16480_set_gyro_scale_custom(adis16480_t *sensor, float new_scale)
{
    sensor->gyro_scale_var = new_scale;
}

/*
    Accel operations
*/

void adis16480_update_acceleration(adis16480_t *sensor)
{
    sensor->x_accl_low = adis16480_read_register(sensor, X_ACCL_LOW);
    sensor->x_accl_out = adis16480_read_register(sensor, X_ACCL_OUT);
    sensor->y_accl_low = adis16480_read_register(sensor, Y_ACCL_LOW);
    sensor->y_accl_out = adis16480_read_register(sensor, Y_ACCL_OUT);
    sensor->z_accl_low = adis16480_read_register(sensor, Z_ACCL_LOW);
    sensor->z_accl_out = adis16480_read_register(sensor, Z_ACCL_OUT);

    if(sensor->dec_rate == 0x00)
    {
        sensor->linear_acceleration[0] = 
            (int32_t)sensor->x_accl_out * sensor->accl_scale_var *
            sensor->accelerometer_dirs[0];
        sensor->linear_acceleration[1] = 
            (int32_t)sensor->y_accl_out * sensor->accl_scale_var *
            sensor->accelerometer_dirs[1];
        sensor->linear_acceleration[2] = 
            (int32_t)sensor->z_accl_out * sensor->accl_scale_var *
            sensor->accelerometer_dirs[2];
    }
}

void adis16480_set_accl_scale_to_ms2(adis16480_t *sensor)
{
    sensor->accl_scale_var = 0.00784532f;
}

void adis16480_set_accl_scale_custom(adis16480_t *sensor, float new_scale)
{
    sensor->accl_scale_var = new_scale;
}

/*
    Magn operations
*/

void adis16480_update_magnetic_course(adis16480_t *sensor)
{
    sensor->x_magn_out = adis16480_read_register(sensor, X_MAGN_OUT);
    sensor->y_magn_out = adis16480_read_register(sensor, Y_MAGN_OUT);
    sensor->z_magn_out = adis16480_read_register(sensor, Z_MAGN_OUT);

    sensor->magnetic_field[0] = (int32_t)sensor->x_magn_out * sensor->magn_scale_var *
    sensor->magnetic_field_dirs[0];
    sensor->magnetic_field[1] = (int32_t)sensor->y_magn_out * sensor->magn_scale_var *
    sensor->magnetic_field_dirs[1];
    sensor->magnetic_field[2] = (int32_t)sensor->z_magn_out * sensor->magn_scale_var *
    sensor->magnetic_field_dirs[2]; 

    sensor->magn_course = atan2(sensor->magnetic_field[1], sensor->magnetic_field[0]);
}

void adis16480_set_magn_scale_to_tesla(adis16480_t *sensor)
{
    sensor->magn_scale_var = 0.0000001f;
}

void adis16480_set_magn_scale_to_gauss(adis16480_t *sensor)
{
    sensor->magn_scale_var = 0.001f;   
}

void adis16480_set_magn_scale_custom(adis16480_t *sensor, float new_scale)
{
    sensor->magn_scale_var = new_scale;
}

void adis16480_update_euler_angles(adis16480_t *sensor)
{
    sensor->roll_c23_out = adis16480_read_register(sensor, ROLL_C23_OUT);
    sensor->pitch_c31_out = adis16480_read_register(sensor, PITCH_C31_OUT);
    sensor->yaw_c32_out = adis16480_read_register(sensor, YAW_C32_OUT);

    sensor->euler_angle[0] = 
        (int32_t)sensor->roll_c23_out * sensor->euler_scale_var;
    sensor->euler_angle[1] = 
        (int32_t)sensor->pitch_c31_out * sensor->euler_scale_var;
    sensor->euler_angle[2] = 
        (int32_t)sensor->yaw_c32_out * sensor->euler_scale_var;    
}

void adis16480_set_euler_scale_to_rads(adis16480_t *sensor)
{
    sensor->euler_scale_var = 0.0000958f;
}

void adis16480_set_euler_scale_to_degs(adis16480_t *sensor)
{
    sensor->euler_scale_var = 0.00549324f;
}

void adis16480_update_pressure(adis16480_t *sensor)
{
    sensor->barom_low = adis16480_read_register(sensor, BAROM_LOW);
    sensor->barom_out = adis16480_read_register(sensor, BAROM_OUT);

    sensor->pressure = sensor->barom_out * sensor->pressure_scale_var;
}

void adis16480_read_seq_cnt(adis16480_t *sensor)
{
    uint16_t answer = adis16480_read_register(sensor, SEQ_CNT);
    sensor->seq_cnt = (answer)&0x3F;
}

void adis16480_read_sys_e_flag(adis16480_t *sensor)
{
    sensor->sys_e_flag.adis_register = adis16480_read_register(sensor, SYS_E_FLAG);
}

void adis16480_read_diag_sts(adis16480_t *sensor)
{
    sensor->diag_sts.adis_register = adis16480_read_register(sensor, DIAG_STS);
}

void adis16480_tare(adis16480_t *sensor)
{
    adis16480_write_register(sensor, GLOB_CMD, 0x100);
}

void adis16480_self_test(adis16480_t *sensor)
{
    // setting bit 2 to apply self-test
    adis16480_write_register(sensor, GLOB_CMD, 0x2);
    // waiting 12 ms
    HAL_Delay(12);
    // reading result state in diag_sts register
    adis16480_read_diag_sts(sensor);
}

void adis16480_get_acceleration(adis16480_t *sensor, float *buffer)
{
    memcpy(buffer, sensor->linear_acceleration, sizeof(sensor->linear_acceleration));
}

void adis16480_get_angular_velocity(adis16480_t *sensor, float *buffer)
{
    memcpy(buffer, sensor->angular_velocity, sizeof(sensor->angular_velocity));
}

void adis16480_get_magnetic_field(adis16480_t *sensor, float *buffer)
{
    memcpy(buffer, sensor->magnetic_field, sizeof(sensor->magnetic_field));
}

void adis16480_get_euler_angles(adis16480_t *sensor, float *buffer)
{
    memcpy(buffer, sensor->euler_angle, sizeof(sensor->euler_angle));
}

float adis16480_get_magnetic_course(adis16480_t *sensor)
{
    return sensor->magn_course;
}

static uint16_t adis16480_read_register(adis16480_t *sensor, uint16_t reg_addr)
{
    uint16_t reg, _data_read;

    // Set page
    reg = 0x8000 | (reg_addr >> 8); // Memory write, Change page
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(sensor->interface, (uint8_t *)&reg, 1, 10);
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

    // Set register
    reg = reg_addr << 8;
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(sensor->interface, (uint8_t *)&reg, 1, 10);
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

    // Read Data
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Receive(sensor->interface, (uint8_t *)&_data_read, 1, 10);
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

    return(_data_read);
}

static HAL_StatusTypeDef __unused_adis16480_write_register(adis16480_t *sensor, uint16_t reg_addr, uint16_t value, uint8_t state)
{
	HAL_StatusTypeDef ret;
	uint16_t reg;
	switch(state)
	{
		// initial stage
		case 0x00:
			// Memory write, Change page
			reg = 0x8000 | (reg_addr >> 8);
    		// send CS low to enable SPI transfer to/from ADIS16480
			HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
			ret = HAL_SPI_Transmit_DMA(sensor->interface, (uint8_t *)&reg, 1);
			break;
		case 0x01:

			break;
	}
}

static HAL_StatusTypeDef adis16480_write_register(adis16480_t *sensor, uint16_t reg_addr, uint16_t value)
{
    HAL_StatusTypeDef ret;
    uint16_t reg;
    // Set page
    reg = 0x8000 | (reg_addr >> 8); // Memory write, Change page
    // send CS low to enable SPI transfer to/from ADIS16480
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
    ret = HAL_SPI_Transmit(sensor->interface, (uint8_t *)&reg, 1, 10);
    // send CS high to disable SPI transfer to/from ADIS16480
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

    // Set register
    reg = 0x8000 | (reg_addr << 8) | (value & 0x00FF);
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
    ret = HAL_SPI_Transmit(sensor->interface, (uint8_t *)&reg, 1, 10);
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

    // Set register
    reg = 0x8000 | ((reg_addr+1) << 8) | (value >> 8);
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_RESET);
    ret = HAL_SPI_Transmit(sensor->interface, (uint8_t *)&reg, 1, 10);
    HAL_GPIO_WritePin(sensor->cs_port, sensor->cs_pin, GPIO_PIN_SET);

    return ret;
}
#endif /*ERROR_RECEIVED*/
