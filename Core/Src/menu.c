#include "menu.h"
#include "ds24b33_manage.h"


uint8_t nested_menu_current_point = 0;

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

void CheckMemHandler(UART_HandleTypeDef *huart)
{
	lcd1602_Clean_Text();
	lcd1602_SetCursor(0, 0);
	lcd1602_Print_text("CHECK MEM");
	lcd1602_SetCursor(0, 1);

	if (check_memory(huart))
	{
		lcd1602_Print_text("SUCCESS");
	}
	else
	{
		lcd1602_Print_text("ERORR");
	}
	HAL_Delay(500);
}

void PrintWriteDataMenu()
{
	extern uint8_t nested_menu_current_point;
	char pointer[LCD1602_LENGTH] = "                ";

	// исключаем выход за значение размера меню
	if (nested_menu_current_point < 0)
	{
		nested_menu_current_point = SIZE_NESTED_MENU;
	}
	else
	{
		nested_menu_current_point = abs(nested_menu_current_point) % SIZE_NESTED_MENU;
	}

	pointer[nested_menu_current_point*5] = '^';

	lcd1602_Clean_Text();
	lcd1602_SetCursor(0, 0);
	lcd1602_Print_text("1    2    3    4");

	lcd1602_SetCursor(0, 1);
	lcd1602_Print_text(pointer);
}

void FirsButtonHandler(UART_HandleTypeDef *huart, MenuManager* main_menu, uint8_t* layer, uint8_t* main_menu_current_point)
{

	if (*layer == 0)
	{
		if (*main_menu_current_point == 0)
		{
			main_menu->menu[*main_menu_current_point].ActionFun(huart);
			PrintMainMenu(main_menu, *main_menu_current_point);
		}
		else
		{
			PrintWriteDataMenu();
			(*layer)++;
		}
	}
	else
	{
		// TODO добавить обработчик
		//main_menu->menu[*main_menu_current_point].ActionFun(huart);
		(*layer)--;

		PrintMainMenu(main_menu, *main_menu_current_point);
	}
}

void SecondButtonHandler(UART_HandleTypeDef *huart, MenuManager* main_menu, uint8_t* layer, uint8_t* main_menu_current_point)
{
	extern uint8_t nested_menu_current_point;

	if (*layer == 1)
	{
		(*layer)--;
		nested_menu_current_point = 0;

		PrintMainMenu(main_menu, *main_menu_current_point);
	}
}

void ThirdButtonHandler(UART_HandleTypeDef *huart, MenuManager* main_menu, uint8_t* layer, uint8_t* main_menu_current_point)
{
	if (*layer == 0)
	{
		(*main_menu_current_point)--;
		*main_menu_current_point = abs(*main_menu_current_point) % main_menu->size; // исключаем выход за значение размера меню

		PrintMainMenu(main_menu, *main_menu_current_point);
	}
	else
	{
		extern uint8_t nested_menu_current_point;
		nested_menu_current_point--;

		PrintWriteDataMenu();
	}
}

void FourthButtonHandler(UART_HandleTypeDef *huart, MenuManager* main_menu, uint8_t* layer, uint8_t* main_menu_current_point)
{
	if (*layer == 0)
	{
		(*main_menu_current_point)++;
		*main_menu_current_point %= main_menu->size; // исключаем выход за значение размера меню

		PrintMainMenu(main_menu, *main_menu_current_point);
	}
	else
	{
		extern uint8_t nested_menu_current_point;
		nested_menu_current_point++;

		PrintWriteDataMenu();
	}
}
