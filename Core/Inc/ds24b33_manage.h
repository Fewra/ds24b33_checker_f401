/*
 * ds24b33_manage.h
 *
 *  Created on: Sep 24, 2024
 *      Author: dimoc
 */

#ifndef INC_DS24B33_MANAGE_H_
#define INC_DS24B33_MANAGE_H_

#include "ds24b33.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

extern void write_data_toMem(UART_HandleTypeDef *huart, uint8_t *data, size_t size, uint16_t addr);
extern bool write_Userdata(UART_HandleTypeDef *huart, const uint8_t *data, size_t size, uint16_t addr);
extern void get_rand_data(uint8_t *data);
extern bool compare_data(uint8_t *write_data, uint8_t *read_data);
extern void read_mem_data(UART_HandleTypeDef *huart, uint8_t *data, size_t size, uint16_t addr);
extern bool check_memory(UART_HandleTypeDef *huart);

#endif /* INC_DS24B33_MANAGE_H_ */
