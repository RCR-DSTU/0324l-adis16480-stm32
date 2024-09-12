#include "adis16480.h"
#ifndef ERROR_RECEIVED
static HAL_StatusTypeDef adis16480_write_register(adis16480_t *sensor, uint16_t reg_addr, uint16_t value);
static uint16_t adis16480_read_register(adis16480_t *sensor, uint16_t reg_addr);

HAL_StatusTypeDef adis16480_init(adis16480_t *sensor,
                    SPI_HandleTypeDef *interface, 
                    GPIO_TypeDef *cs_port,
                    uint16_t cs_pin)
{
    sensor->interface = interface;
    sensor->cs_port = cs_port;
    sensor->cs_pin = cs_pin;

    sensor->prod_id = 0x00;

    sensor->prod_id = adis16480_read_register(sensor, PROD_ID);
    if(sensor->prod_id != 0x4060) return HAL_ERROR;

    sensor->dec_rate = adis16480_read_register(sensor, DEC_RATE);

    // initial scaling to m/s^2
    sensor->accl_scale_var = 0.00784f;
    // initial scaling to deg/s
    sensor->gyro_scale_var = 0.02f;
    // initial scaling to gauss
    sensor->magn_scale_var = 0.0001f;
    // initial scaling to degrees
    sensor->euler_scale_var = 0.00549324f;
    return HAL_OK;
}

void adis16480_end_calibration(adis16480_t *sensor)
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
            sensor->x_gyro_out * sensor->gyro_scale_var;
        sensor->angular_velocity[1] = 
            sensor->y_gyro_out * sensor->gyro_scale_var;
        sensor->angular_velocity[2] = 
            sensor->z_gyro_out * sensor->gyro_scale_var;
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
            (int32_t)sensor->x_accl_out * sensor->accl_scale_var;
        sensor->linear_acceleration[1] = 
            (int32_t)sensor->y_accl_out * sensor->accl_scale_var;
        sensor->linear_acceleration[2] = 
            (int32_t)sensor->z_accl_out * sensor->accl_scale_var;
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

    sensor->magnetic_field[0] = sensor->x_magn_out * sensor->magn_scale_var;
    sensor->magnetic_field[1] = sensor->y_magn_out * sensor->magn_scale_var;
    sensor->magnetic_field[2] = sensor->z_magn_out * sensor->magn_scale_var; 

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
