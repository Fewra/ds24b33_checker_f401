/*
 * menu.h
 *
 *  Created on: Sep 12, 2024
 *      Author: dimoc
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include <stdint.h>
#include "lcd1602_i2c_lib.h"

typedef enum {
	Ok,
	Back,
	Left,
	Right
}Action;

typedef struct {
	char name[13];
	uint8_t size;
	int (*ActionFun)(Action);
} IMenu;

typedef struct {
	IMenu menu[2];
} MenuManager;


extern void PrintMenu(MenuManager* m, uint8_t point);

#endif /* INC_MENU_H_ */
