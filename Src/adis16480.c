#include "adis16480.h"

uint8_t adis16480_init(adis16480_t *object,
                    SPI_HandleTypeDef *interface, 
                    GPIO_TypeDef *port,
                    uint16_t pin)
{
    object->interface = interface;
    object->cs_port = port;
    object->cs_pin = pin;

    object->prod_id = 0x00;

    object->prod_id = adis16480_read_register(object, PROD_ID);
    if(object->prod_id != 0x4060) return 0x01;

}

void adis16480_reset(adis16480_t *object)
{
    uint16_t reg = 0x8003;

    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(object->interface, (uint8_t *)&reg, 1, 10);
}

void adis16480_update_acceleration(adis16480_t *object)
{
    uint16_t ans_out, ans_low;
    ans_low = adis16480_read_register(object, Z_ACCL_LOW);
    ans_out = adis16480_read_register(object, Z_ACCL_OUT);

    // object->Accl[0] = adis16480_read_register(object, X_ACCL_OUT);
    // object->Accl[1] = adis16480_read_register(object, Y_ACCL_OUT);
    // object->Accl[2] = adis16480_read_register(object, Z_ACCL_OUT);

    // object->Accl_f[0] = ((float)object->Accl[0]) * 0.0078408;
    // object->Accl_f[1] = ((float)object->Accl[1]) * 0.0078408;
    // object->Accl_f[2] = ((float)object->Accl[2]) * 0.0078408;
}

void adis16480_update_angular_velocity(adis16480_t *object)
{
	int16_t RawH[3];

	RawH[0] = adis16480_read_register(object, X_GYRO_OUT);
	RawH[1] = adis16480_read_register(object, Y_GYRO_OUT);
	RawH[2] = adis16480_read_register(object, Z_GYRO_OUT);
	// RawL[0] = adis16480_read_register(object, X_GYRO_LOW);
	// RawL[1] = adis16480_read_register(object, Y_GYRO_LOW);
	// RawL[2] = adis16480_read_register(object, Z_GYRO_LOW);
	
	// object->Gyro_f[0] = ((float)RawH[0]) * 0.02;
	// object->Gyro_f[1] = ((float)RawH[1]) * 0.02;
	// object->Gyro_f[2] = ((float)RawH[2]) * 0.02;
}

void adis16480_update_magnetic_field(adis16480_t *object)
{
    // object->Magn[0] = adis16480_read_register(object, X_MAGN_OUT);
    // object->Magn[1] = adis16480_read_register(object, Y_MAGN_OUT);
    // object->Magn[2] = adis16480_read_register(object, Z_MAGN_OUT);   

    // object->Magn_f[0] = ((float)object->Magn[0]) * 0.0001;
    // object->Magn_f[1] = ((float)object->Magn[1]) * 0.0001;
    // object->Magn_f[2] = ((float)object->Magn[2]) * 0.0001;

}

void adis16480_read_seq_cnt(adis16480_t *object)
{
    uint16_t answer = 0x00;
    answer = adis16480_read_register(object, SEQ_CNT);
    object->seq_cnt = answer;
}

void adis16480_read_sys_e_flag(adis16480_t *object)
{
    uint16_t answer = 0x00;
    answer = adis16480_read_register(object, SYS_E_FLAG);

    object->sys_e_flag.watchdog_flag = (unsigned int)(answer&0x8000) >> 15;
    object->sys_e_flag.ekf_divergence = (unsigned int)(answer&0x2000) >> 13;
    object->sys_e_flag.gyroscope_saturation = (unsigned int)(answer&0x1000) >> 12;
    object->sys_e_flag.magnetometer_disturbance = (unsigned int)(answer&0x800) >> 11;
    object->sys_e_flag.acceleration_threshold_flag = (unsigned int)(answer&0x400) >> 10;
    object->sys_e_flag.barometer_new_data = (unsigned int)(answer&0x200) >> 9;
    object->sys_e_flag.magnetometer_new_data = (unsigned int)(answer&0x100) >> 8;
    object->sys_e_flag.processing_overrun = (unsigned int)(answer&0x80) >> 7;
    object->sys_e_flag.flash_memory_update_flag = (unsigned int)(answer&0x40) >> 6;
    object->sys_e_flag.inertial_self_test_flag = (unsigned int)(answer&0x20) >> 5;
    object->sys_e_flag.sensor_overrange = (unsigned int)(answer&0x10) >> 4;
    object->sys_e_flag.spi_connection_error =(unsigned int)(answer&0x8) >> 3;
    object->sys_e_flag.alarm_status_flag = (unsigned int)(answer&0x1);
}

void adis16480_read_diag_sts(adis16480_t *object)
{



}

// void adis16480_tare(adis16480_t *object)
// {

// }

// void adis16480_self_test(adis16480_t *object)
// {

// }

// void adis16480_set_body_frame(adis16480_t *object)
// {

// }

// void adis16480_set_dec_rate(uint16_t value)
// {

// }

// void adis16480_set_ref_matrix(int16_t *matrix)
// {

// }

uint16_t adis16480_read_page(adis16480_t *object)
{
    uint16_t reg = 0, _dataRead;
    // Set page
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(object->interface, (uint8_t *)&reg, 1, 10); // write to GLOB_CMD upper byte
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_SET);

    // Read Data
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Receive(object->interface, (uint8_t *)&_dataRead, 1, 10);
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_SET);

    return(_dataRead);
}

uint16_t adis16480_read_register(adis16480_t *object, uint16_t reg_addr)
{
    uint16_t reg, _data_read;

    // Set page
    reg = 0x8000 | (reg_addr >> 8); // Memory write, Change page
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(object->interface, (uint8_t *)&reg, 1, 10);
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_SET);

    // Set register
    reg = reg_addr << 8;
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(object->interface, (uint8_t *)&reg, 1, 10);
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_SET);

    // Read Data
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Receive(object->interface, (uint8_t *)&_data_read, 1, 10);
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_SET);

    return(_data_read);
}

void adis16480_write_register(adis16480_t *object, uint16_t reg_addr, uint16_t value)
{
    uint16_t reg;	
    // Set page
    reg = 0x8000 | (reg_addr >> 8); // Memory write, Change page
    // send CS low to enable SPI transfer to/from ADIS16480
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(object->interface, (uint8_t *)&reg, 1, 10);
    // send CS high to disable SPI transfer to/from ADIS16480
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_SET);

    // Set register
    reg = 0x8000 | (reg_addr << 8) | (value & 0x00FF);
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(object->interface, (uint8_t *)&reg, 1, 10);
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_SET);

    // Set register
    reg = 0x8000 | ((reg_addr+1) << 8) | (value >> 8);
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(object->interface, (uint8_t *)&reg, 1, 10);
    HAL_GPIO_WritePin(object->cs_port, object->cs_pin, GPIO_PIN_SET);
}
