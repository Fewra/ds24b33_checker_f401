/*
 * ds24b33_manage.c
 *
 *  Created on: Sep 24, 2024
 *      Author: dimoc
 */

#include "ds24b33_manage.h"

static void write_data_toMem(UART_HandleTypeDef *huart, uint8_t *data, size_t size, uint16_t addr)
{
	uint8_t TA1 = (addr & 0x00FF);
	uint8_t TA2 = ((addr & 0xFF00)>>8);

	uint8_t dataRom[8];

	uint8_t writeScratch_data[size+2];
	uint8_t readScratch_data[35];
	uint8_t copyScratch_data[3];

	writeScratch_data[0] = TA1;
	writeScratch_data[1] = TA2;
	for (size_t i = 2; i < size+2; i++)
		writeScratch_data[i] = data[i-2];

	// сначала делаем ReadRom, а потом другие команды
	ReadRom(huart, dataRom);
	WriteScratchpad(huart, writeScratch_data, size+2);

	ReadRom(huart, dataRom);
	ReadScratchpad(huart, readScratch_data);

	// запись целевого адреса в начало массивов
	for (int i = 0; i < 3; i++)
		copyScratch_data[i] = readScratch_data[i];

	ReadRom(huart, dataRom);
	CopyScratchpad(huart, copyScratch_data);
}

bool write_Userdata(UART_HandleTypeDef *huart, const uint8_t *data, size_t size, uint16_t addr)
{
	if(addr + size - 1 > 0x1FF)
		return false;

	if (addr < 0x0 || addr > 0x1FF)
		return false;

	for (size_t i = 0, size_arr = 32 - (addr % 32); i < size;)
		{
			if (size_arr > (size-i))
				size_arr = (size-i) % 32;

			uint8_t part_data[size_arr];

			for (uint8_t j = 0; j < size_arr; j++)
				part_data[j] = data[i+j];

			// запись в память
			write_data_toMem(huart, part_data, size_arr, addr);

			addr += size_arr;
			i += size_arr;

			if (size-i >= 32)
				size_arr = 32;
			else
			{
				size_arr = (size-i) % 32;
				if (size_arr == 0)
					break;
			}
		}

	return true;
}


static void get_rand_data(uint8_t *data)
{
	for (uint8_t i=0; i<32; i++)
	{
		data[i] = (rand() % 256);
	}
}

static bool compare_data(uint8_t *write_data, uint8_t *read_data)
{
	for (uint8_t i = 0; i < 32; i++)
	{
		if (write_data[i] != read_data[i])
			return memory_false;
	}
	return memory_true;
}

static void read_mem_data(UART_HandleTypeDef *huart, uint8_t *data, size_t size, uint16_t addr)
{
	uint8_t TA1 = (addr & 0x00FF);
	uint8_t TA2 = ((addr & 0xFF00)>>8);
	uint8_t buf[size+2];

	buf[0] = TA1;
	buf[1] = TA2;

	uint8_t dataRom[8];

	ReadRom(huart, dataRom);
	ReadMemory(huart, buf, size);

	for(size_t i = 0; i < size; i++)
		data[i] = buf[i];
}

bool check_memory(UART_HandleTypeDef *huart)
{
	uint8_t data[32] = {0};
	uint8_t readMem_data[32] = {0};

	for (uint32_t addr = 0; addr <= 0x01E0; addr+=32)
	{

		get_rand_data(data);
		write_data_toMem(huart, data, 32, addr);
		read_mem_data(huart, readMem_data, 32, addr);

		if (!compare_data(data, readMem_data))
			return memory_false;
	}

	return memory_true;
}
