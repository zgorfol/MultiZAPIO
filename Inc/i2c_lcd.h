/**
  ******************************************************************************
  * @file		i2c_lcd.h
  * @author		zgorfol
  * @date		1 Aug 2020
  ******************************************************************************
  */


#ifndef BIOZAP_LCD_H_
#define BIOZAP_LCD_H_


  /** Includes ---------------------------------------------------------------- */

#include <string>
#include <sstream>
#include <biozap_comm.h>

#define LCD_ADDR (0x27 << 1)

#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)

#define LCD_DELAY_MS 5

#define LCD_LineLg 16

void I2C_Scan(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart);
void LCD_Init(I2C_HandleTypeDef *hi2c, uint8_t lcd_addr);
void LCD_SendCommand(I2C_HandleTypeDef *hi2c, uint8_t lcd_addr, uint8_t cmd);
void LCD_SendString(I2C_HandleTypeDef *hi2c, uint8_t lcd_addr, std::string str);
void init_LCD(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart, std::string lcd_msg);


#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/

