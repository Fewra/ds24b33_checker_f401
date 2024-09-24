#include "menu.h"

// добавление пунктов вложенного меню
uint8_t nested_menu_current_point = 0;
MenuManager choice_firm;

// добавление символов, чтобы пользователь понимал, какой пункт главного меню он выбрал
void GetChoisePoint(char* mes, uint8_t size_mes, char* choise)
{
	for (uint8_t i = 0; i < size_mes; i++)
	{
		choise[i+1] = mes[i];
	}

	choise[0] = '<';
	choise[size_mes] = '>';
	choise[size_mes+1] = '\0';
}

// Вывод пунктов главного меню
void PrintMainMenu(MenuManager* m, uint8_t point)
{
	uint8_t menu_size = sizeof(m->menu) / sizeof(m->menu[0]);

	char* first_msg;
	char* second_msg;

	char user_choice[16];

	GetChoisePoint(m->menu[point].name, m->menu[point].size, user_choice);

	if (point == menu_size-1)
	{
		first_msg = m->menu[point-1].name;
		second_msg = user_choice;
	}
	else
	{
		first_msg = user_choice;
		second_msg = m->menu[point+1].name;
	}

	lcd1602_Clean_Text();

	lcd1602_SetCursor(0, 0);
	lcd1602_Print_text(first_msg);

	lcd1602_SetCursor(0, 1);
	lcd1602_Print_text(second_msg);
}

void FirsButtonHandler(MenuManager* main_menu, uint8_t layer, uint8_t current_point)
{
	if (layer == 0)
	{
		switch(current_point)
		{
		case 0:

			break;
		case 1:

			break;
		}
	}
	else
	{

	}
}

void SecondButtonHandler(MenuManager* main_menu, uint8_t layer, uint8_t current_point)
{
	if (layer == 1)
	{

	}
}

void ThirdButtonHandler(MenuManager* main_menu, uint8_t layer, uint8_t current_point)
{
	if (layer == 0)
	{

	}
}

void FourthButtonHandler(MenuManager* main_menu, uint8_t layer, uint8_t current_point)
{
	if (layer == 0)
	{
		PrintMainMenu(main_menu, current_point);
	}
}
