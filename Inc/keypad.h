/**
  ******************************************************************************
  * @file		keypad.h
  * @author	based Yohanes Erwin Setiawan
  * @modified	zgorfol
  * @date		1 Aug 2020
  ******************************************************************************
  */

#ifndef __KEYPAD_H
#define __KEYPAD_H

#ifdef __cplusplus
extern "C" {
#endif

/** Includes ---------------------------------------------------------------- */
#include "main.h"

// Configure GPIO as output pull up for keypad columns
#define KEYPAD_GPIO_COL0			D6_GPIO_Port
#define KEYPAD_PIN_COL0				D6_Pin
#define KEYPAD_GPIO_COL1			D5_GPIO_Port
#define KEYPAD_PIN_COL1				D5_Pin
#define KEYPAD_GPIO_COL2			D4_GPIO_Port
#define KEYPAD_PIN_COL2				D4_Pin
#define KEYPAD_GPIO_COL3			D3_GPIO_Port
#define KEYPAD_PIN_COL3				D3_Pin

// Configure GPIO as input with pull-down resistor for keypad rows
// You have to edit stm32..it.c to write EXTI... handlers
#define KEYPAD_GPIO_ROW0			D10_GPIO_Port
#define KEYPAD_PIN_ROW0				D10_Pin
#define KEYPAD_PIN_ROW0_EXTI	EXTI15_10_IRQn
#define KEYPAD_GPIO_ROW1			D9_GPIO_Port
#define KEYPAD_PIN_ROW1				D9_Pin
#define KEYPAD_PIN_ROW1_EXTI	EXTI9_5_IRQn
#define KEYPAD_GPIO_ROW2			D8_GPIO_Port
#define KEYPAD_PIN_ROW2				D8_Pin
#define KEYPAD_PIN_ROW2_EXTI	EXTI15_10_IRQn
#define KEYPAD_GPIO_ROW3			D7_GPIO_Port
#define KEYPAD_PIN_ROW3				D7_Pin
#define KEYPAD_PIN_ROW3_EXTI	EXTI15_10_IRQn

#define KEYPAD_NO_PRESSED			0x00


char KeypadGetKey(bool withIT);
char getKeyP();
void init_Keypad(bool withIT, TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
