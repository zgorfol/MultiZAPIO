/*
 * i2c_lcd.cpp
 *
 *  Created on: Sep 6, 2020
 *      Author: zgorfol
 */

#include <main.h>
#include <i2c_lcd.h>

using namespace std;

extern string EOL;
//extern string lcd_msg;

bool is_I2C = false;

void I2C_Scan(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart) {
    string info = "Scanning I2C bus...\r\n";
    uart_TX_IT(huart, info);

    HAL_StatusTypeDef res;
    for(uint16_t i = 0; i < 128; i++) {
        res = HAL_I2C_IsDeviceReady(hi2c, i << 1, 1, 10);
        if(res == HAL_OK) {
        	std::stringstream ss;
        	string msg = "";
        	ss << hex << (int)( i );
        	ss >> msg;
            uart_TX_IT(huart, " 0x" + msg + " ");
            is_I2C  = true;
        } else {
        	uart_TX_IT(huart, ".");
        }
    }

    uart_TX_IT(huart, EOL+">");
}

HAL_StatusTypeDef LCD_SendInternal(I2C_HandleTypeDef *hi2c, uint8_t lcd_addr, uint8_t data, uint8_t flags) {
    HAL_StatusTypeDef res;
    if(!is_I2C) {
    	return HAL_ERROR;
    }
    for(;;) {
        res = HAL_I2C_IsDeviceReady(hi2c, lcd_addr, 1, HAL_MAX_DELAY);
        if(res == HAL_OK)
            break;
    }

    uint8_t up = data & 0xF0;
    uint8_t lo = (data << 4) & 0xF0;

    uint8_t data_arr[4];
    data_arr[0] = up|flags|BACKLIGHT|PIN_EN;
    data_arr[1] = up|flags|BACKLIGHT;
    data_arr[2] = lo|flags|BACKLIGHT|PIN_EN;
    data_arr[3] = lo|flags|BACKLIGHT;

    res = HAL_I2C_Master_Transmit(hi2c, lcd_addr, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
    HAL_Delay(LCD_DELAY_MS);
    return res;
}

void LCD_SendCommand(I2C_HandleTypeDef *hi2c, uint8_t lcd_addr, uint8_t cmd) {
    LCD_SendInternal(hi2c, lcd_addr, cmd, 0);
}

void LCD_SendData(I2C_HandleTypeDef *hi2c, uint8_t lcd_addr, uint8_t data) {
    LCD_SendInternal(hi2c, lcd_addr, data, PIN_RS);
}

void LCD_Init(I2C_HandleTypeDef *hi2c, uint8_t lcd_addr) {
    // 4-bit mode, 2 lines, 5x7 format
    LCD_SendCommand(hi2c, lcd_addr, 0b00110000);
    // display & cursor home (keep this!)
    LCD_SendCommand(hi2c, lcd_addr, 0b00000010);
    // display on, right shift, underline off, blink off
    LCD_SendCommand(hi2c, lcd_addr, 0b00001100);
    // clear display (optional here)
    LCD_SendCommand(hi2c, lcd_addr, 0b00000001);
}

void LCD_SendString(I2C_HandleTypeDef *hi2c, uint8_t lcd_addr, string str) {
	for(uint i = 0; i < str.length(); i++){
		LCD_SendData(hi2c, lcd_addr, (uint8_t)str[i]);
	}
}


void init_LCD(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart, string lcd_msg)
{
    I2C_Scan(hi2c, huart);
    LCD_Init(hi2c, LCD_ADDR);

    // set address to 0x00
    LCD_SendCommand(hi2c, LCD_ADDR, 0b10000000);
    LCD_SendString(hi2c, LCD_ADDR, lcd_msg);

    // set address to 0x40
    LCD_SendCommand(hi2c, LCD_ADDR, 0b11000000);
//    LCD_SendString(hi2c, LCD_ADDR, "  over I2C bus");
}

