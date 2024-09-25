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

extern bool write_Userdata(UART_HandleTypeDef *huart, const uint8_t *data, size_t size, uint16_t addr);
extern bool check_memory(UART_HandleTypeDef *huart);

#endif /* INC_DS24B33_MANAGE_H_ */
