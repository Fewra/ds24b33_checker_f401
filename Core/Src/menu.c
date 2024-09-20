#include "menu.h"

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

void PrintMenu(MenuManager* m, uint8_t point)
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

	lcd1602_SetCursor(0, 0);
	lcd1602_Print_text(first_msg);

	lcd1602_SetCursor(0, 1);
	lcd1602_Print_text(second_msg);
}
