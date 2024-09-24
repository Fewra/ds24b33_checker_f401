/*
 * menu.h
 *
 *  Created on: Sep 12, 2024
 *      Author: dimoc
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include <stdint.h>
#include <stdbool.h>
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

// если добавляем пункты меню, то меняем размер поля IMenu
typedef struct {
	IMenu menu[2];
	uint8_t size;
} MenuManager;


extern void PrintMainMenu(MenuManager* m, uint8_t point);
extern void FirsButtonHandler(MenuManager* main_menu, uint8_t layer, uint8_t current_point);
extern void SecondButtonHandler(MenuManager* main_menu, uint8_t layer, uint8_t current_point);
extern void ThirdButtonHandler(MenuManager* main_menu, uint8_t layer, uint8_t current_point);
extern void FourthButtonHandler(MenuManager* main_menu, uint8_t layer, uint8_t current_point);

#endif /* INC_MENU_H_ */
