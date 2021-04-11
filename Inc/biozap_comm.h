/*
 * biozap_comm.h
 *
 *  Created on: Sep 6, 2020
 *      Author: zgorfol
 */

#ifndef BIOZAP_COMM_H_
#define BIOZAP_COMM_H_


void uart_TX_IT(UART_HandleTypeDef *huart, std::string inputString);
void Cmd_Interpreter(UART_HandleTypeDef *huart, DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, I2C_HandleTypeDef *hi2c);
void KiIratas(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart, std::__cxx11::string KiString, bool prevKiir);

#endif /* BIOZAP_COMM_H_ */
