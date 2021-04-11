/*
 * biozap_prog.cpp
 *
 *  Created on: Ápr 9, 2021
 *      Author: zgorfol
 */

#include "main.h"
#include "biozap_prog.h"
#include <string>
#include <list>
using namespace std;

extern uint32_t __userData_start__;                // from the linkerscript
extern uint32_t __Flash_PageSize__;                // from the linkerscript
extern uint32_t g_pfnVectors[];                    // true array (vector table of all ISRs), from the startup assembly .s file
uint32_t FLASH_START = (uint32_t)&g_pfnVectors[0]; // Get the address of the first element of this array and cast it to a 4-byte unsigned integer
uint32_t FLASH_STORAGE = (uint32_t)&__userData_start__;
uint32_t PAGE_SIZE = (uint32_t)&__Flash_PageSize__;

char *eeprom_Data_Chr = (char *)FLASH_STORAGE;

void read_flash(string *data)
{
    volatile uint32_t read_data;
    volatile uint32_t read_cnt = 0;
    do
    {
        read_data = *(uint32_t *)(FLASH_STORAGE + read_cnt);
        if (read_data != 0xFFFFFFFF)
        {
            *data += (char)read_data;
            *data += (char)(read_data >> 8);
            *data += (char)(read_data >> 16);
            *data += (char)(read_data >> 24);
            read_cnt += 4;
        }
    } while ((read_data != 0xFFFFFFFF) && (read_cnt < PAGE_SIZE - 1));
    while ((data->length() > 0) && (data->at(data->length() - 1) != '\n'))
    {
        data->pop_back(); // Delete any extra char before ending LF
    }
}

void save_to_flash(storedprogram data)
{
    volatile uint32_t data_length = (data.item.length() / 8) + (int)((data.item.length() % 8) != 0);
    volatile uint64_t data_to_FLASH[data_length];
    memset((uint8_t *)data_to_FLASH, 0, sizeof((char *)data_to_FLASH));
    strcpy((char *)data_to_FLASH, data.item.c_str());

    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Page = (FLASH_STORAGE - FLASH_START) / PAGE_SIZE;
    volatile uint16_t pages = (data.item.length() / PAGE_SIZE) + (int)((data.item.length() % PAGE_SIZE) != 0);
    EraseInitStruct.NbPages = pages;
    uint32_t PageError;
    volatile HAL_StatusTypeDef status;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
        Error_Handler();
        return;
    }

    volatile uint32_t write_cnt = 0, index = 0;
    while (index < data_length)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_STORAGE + write_cnt, data_to_FLASH[index]);
        if (status == HAL_OK)
        {
            write_cnt += 8;
            index++;
        }
        else
        {
            Error_Handler();
            return;
        }
    }
    HAL_FLASH_Lock();
}

void save_string_to_flash(string data)
{
    volatile uint32_t data_length = (data.length() / 8) + (int)((data.length() % 8) != 0);
    volatile uint64_t data_to_FLASH[data_length];
    memset((uint8_t *)data_to_FLASH, 0, sizeof((char *)data_to_FLASH));
    strcpy((char *)data_to_FLASH, data.c_str());

    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Page = (FLASH_STORAGE - FLASH_START) / PAGE_SIZE;
    volatile uint16_t pages = (data.length() / PAGE_SIZE) + (int)((data.length() % PAGE_SIZE) != 0);
    EraseInitStruct.NbPages = pages;
    uint32_t PageError;
    volatile HAL_StatusTypeDef status;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
        Error_Handler();
        return;
    }

    volatile uint32_t write_cnt = 0, index = 0;
    while (index < data_length)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_STORAGE + write_cnt, data_to_FLASH[index]);
        if (status == HAL_OK)
        {
            write_cnt += 8;
            index++;
        }
        else
        {
            Error_Handler();
            return;
        }
    }
    HAL_FLASH_Lock();
}

/*
  const program userProgram = {
   		"#Earth rhythm 8m\n"
  		"wait 3000\n"
  		"beep 100\n"
  		"freq 783 480\n"
  		"beep 500\n"
  		"off\n"
  };
  save_to_flash(userProgram); // If userProgram too big change the DATA size,
   														// now 2k the size in the STM32L432KCUx_FLASH.ld !!!
*/