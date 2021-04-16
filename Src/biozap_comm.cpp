/*
 * biozap_comm.cpp
 *
 *  Created on: Sep 6, 2020
 *      Author: zgorfol
 */

#include "main.h"
#include <string>
#include <list>
using namespace std;

#include <cstring>
#include <sstream>

#include "i2c_keypad.h"
#include "i2c_lcd.h"
#include "biozap_comm.h"
#include "biozap_freq.h"
#include "biozap_prog.h"
using namespace std;

#define MAX_CMD_PARAMS 3

struct paramstruc{
	string command;
	string paramStr[MAX_CMD_PARAMS];
	uint32_t paramNum[MAX_CMD_PARAMS];
};

struct struct_commands{ 
	string command;
	short int paramCount;
 };

#define COMMS_NUMBER  17

 struct_commands legal_commands[COMMS_NUMBER] = {
	 {"freq", 2},
	 {"exe", 1},
	 {"ls", 0},
	 {"vout", 1},
	 {"vmin", 1},
	 {"beep", 1},
	 {"pbar", 0},
	 {"#", 0},
	 {":", 0},
	 {"mem", 0},
	 {"@", 0},
	 {"Eszti", 0},
	 {"wait", 1},
	 {"off", 0},
	 {"abort", 0},
	 {"signal", 1},
	 {"chp", 1},
 };

 uint16_t vout = 1200;
 uint16_t vmin = 0;
 uint8_t  signal_form = BIOZAP_SIN;

 volatile bool command_arrived = false;
 volatile bool command1_arrived = false;
 volatile bool uart_TX_busy = false;
 volatile bool uart_TX1_busy = false;
 volatile bool abort_Prog_Run = false; // Abort Command Interpreter running

 char LF = '\n';
 char CR = '\r';
 string EOL = "\r\n";

 uint8_t rx_data; // Serial receive char
 string tx_buffer = "";
 string rx_buffer = "";	   // Serial receive buffer
 list<string> command_Line = {}; // arrived command line

 uint8_t rx1_data; // Serial receive char
 string tx1_buffer = "";
 string rx1_buffer = "";	// Serial receive buffer
 list<string> command_Line1 = {}; // arrived command line

 string user_Program = ""; // Store user program

 void uart_TX_IT(UART_HandleTypeDef *huart, string inputString)
 {
	if(inputString.length() == 0) return; 
	if (huart->Instance == USART2) {
		while (uart_TX_busy) { // Wait empty TX buffer
			 ;
		}
		tx_buffer = "";
		tx_buffer += inputString;
		HAL_UART_Transmit_IT(huart, (uint8_t *)tx_buffer.c_str(), tx_buffer.length());
		uart_TX_busy = true;
	}
	if (huart->Instance == USART1) {
		while (uart_TX1_busy)
		{ // Wait empty TX buffer
			;
		}
		tx1_buffer = "";
		tx1_buffer += inputString;
		HAL_UART_Transmit_IT(huart, (uint8_t *)tx1_buffer.c_str(), tx1_buffer.length());
		uart_TX1_busy = true;
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2) {
		uart_TX_busy = false;
	}
	if (huart->Instance == USART1) {
		uart_TX1_busy = false;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2) {
		if(EOL.find(rx_data) != std::string::npos){ // CR or LF
			if(rx_buffer.length() > 0){
				//command_Line = "";
				command_Line.push_back(rx_buffer+EOL);
				rx_buffer = "";
				std::list<string>::iterator idx = command_Line.end();
				abort_Prog_Run = *(--idx) == "abort"+EOL;
				command_arrived = true;  // Start Command Interpreter
			}
		}
		else {
			rx_buffer += rx_data;
		}
		HAL_UART_Receive_IT(huart,&rx_data, 1);
	}
	if (huart->Instance == USART1) {
		if(EOL.find(rx1_data) != std::string::npos){ // CR or LF
			if(rx1_buffer.length() > 0){
				//command_Line1 = "";
				command_Line1.push_back(rx1_buffer+EOL);
				rx1_buffer = "";
				std::list<string>::iterator idx = command_Line1.end();
				abort_Prog_Run = *(--idx) == "abort" + EOL;
				command1_arrived = true;  // Start Command Interpreter
			}
		}
		else {
			rx1_buffer += rx1_data;
		}
		HAL_UART_Receive_IT(huart,&rx1_data, 1);
	}
}


void start_DMA(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, uint16_t psc, uint16_t arr)
{
	extern int BIOZAP_Sample_Lgth;
	extern uint16_t BIOZAP_SampleArray[];

	BIOZAP_Sample_Lgth = min(psc, (uint16_t)BIOZAP_SAMPLE_SIZE);
	generate_sample(vmin, vout - vmin, signal_form, BIOZAP_SampleArray);
	HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_1, (uint32_t *)BIOZAP_SampleArray, BIOZAP_Sample_Lgth, DAC_ALIGN_12B_R);
//	HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_2, (uint32_t *)BIOZAP_SampleArray, BIOZAP_Sample_Lgth, DAC_ALIGN_12B_R);

	__HAL_TIM_SET_PRESCALER(htim, 0);
	__HAL_TIM_SET_AUTORELOAD(htim, arr);
	HAL_TIM_Base_Start(htim);
}

void stop_DMA(DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim)
{
	HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_1);
//	HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_2);
	HAL_TIM_Base_Stop(htim);
}


void HAL_DAC_DMAUnderrunCallbackCh1(DAC_HandleTypeDef *hdac)
{
	__HAL_DAC_DISABLE(hdac, DAC_CHANNEL_1);
	hdac->ErrorCode = 0;
	hdac->State = HAL_DAC_STATE_READY;
	__HAL_DAC_ENABLE(hdac, DAC_CHANNEL_1);
}

void findAndReplaceAll(std::string &data, std::string toSearch, std::string replaceStr)
{
	//if (data == "") return;
	size_t pos = data.find(toSearch); // Get the first occurrence
	while (pos != std::string::npos)
	{													// Repeat till end is reached
		data.replace(pos, toSearch.size(), replaceStr); // Replace Sub String
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

string prevLcdString = "";

string formatLCDStr(string beStr) {
	string wStr = beStr.substr(0, LCD_LineLg);
	findAndReplaceAll(wStr, "\r", "");
	findAndReplaceAll(wStr, "\n", "");
	wStr.append(LCD_LineLg - wStr.length(), ' ');
	return wStr;
}

void KiIratas(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart, std::__cxx11::string KiString, bool prevKiir)
{
	if (prevKiir) {
		LCD_SendCommand(hi2c, LCD_ADDR, 0b10000000);
		LCD_SendString(hi2c, LCD_ADDR, prevLcdString);
		prevLcdString = formatLCDStr(KiString);
	}

	LCD_SendCommand(hi2c, LCD_ADDR, 0b11000000);
	LCD_SendString(hi2c, LCD_ADDR, formatLCDStr(KiString));
	uart_TX_IT(huart, KiString);
}

void Delay(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart, uint32_t Del_ms)
{
	uint32_t Del_sec = Del_ms / 1000;

	if(Del_sec < 1) {
		HAL_Delay(Del_ms);
		return;
	}
	for (uint16_t idx = Del_sec; idx > 0; idx--)
	{
		for(uint16_t i = 0; i < 10; i ++) {
			HAL_Delay(100);
			char pressedKey = readKeyboard();
			if (pressedKey == 'A') {
				abort_Prog_Run = true;
				return;
			}
		}
		char pressedKey = readKeyboard();
		if (pressedKey == 'A')
		{
			abort_Prog_Run = true;
			return;
		}
		KiIratas(hi2c, huart, to_string(idx) + "     \r", false);
		if(abort_Prog_Run)
			return;
	}
}

bool has_only_digits(string s) {
	return s.find_first_not_of("0123456789") == string::npos;
}

void param_Conv(paramstruc *param, int num)
{
	int i = 0;
	while(i < num)	{
		if(has_only_digits(param->paramStr[i])) {
			param->paramNum[i] = std::stol(param->paramStr[i]);
			//	param->paramNum[i] = strtol(param->paramStr[i].c_str(), NULL, 10);
		}
		i++;
	}
}

bool getParams(string inputString, paramstruc *param, UART_HandleTypeDef *huart, I2C_HandleTypeDef *hi2c)
{
	int j = 0;
	param->command = "";
	for (int i = 0; i < MAX_CMD_PARAMS; i++) {
		param->paramNum[i] = 0;
		param->paramStr[i] = "";
	}
	string wString = "";
	for (uint8_t i = 0; i < inputString.length(); i++){
		if((inputString[i] == 32) || (inputString [i] == 10) || (inputString[i] == 13)) {
			/*
			if (inputString[i] == 32) {
				if(++j == MAX_CMD_PARAMS) {
					KiIratas(huart, "Bad command, too many space!!!" + EOL);
					return false;
				}
				else {
					param->param[j-1] = std::stol(wString);
					wString = "";
				}
			}
			*/
			if(wString.compare("") != 0) {
				if(param->command.compare("") == 0)
					param->command = wString;
				else {
					if (j == MAX_CMD_PARAMS) {
						KiIratas(hi2c, huart, "Bad command, too many space!!!" + EOL, true);
						return false;
					}
					else {
					//	param->param[j] = std::stol(wString);
					param->paramStr[j] = wString;
					}
					j++;
				}
				wString = "";
				
			}
		}
		else {
			wString += inputString[i];
			//param->param[j] += inputString[i];
		}
	}
	for (int i = 0; i < COMMS_NUMBER; i++){
		if ((param->command.find(legal_commands[i].command) != std::string::npos) &&
			((legal_commands[i].paramCount == j) || (legal_commands[i].paramCount == 0)))
		{
			param_Conv(param, legal_commands[i].paramCount);
			return true;
		}
	}
	return false;
}

void beep(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef *huart, uint32_t delay_time)
{
	HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);
	//HAL_Delay(delay_time);
	Delay(hi2c, huart, delay_time);
	HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);
}

bool store_Program = false;

void Command_Interpreter(string comm_Str, UART_HandleTypeDef *huart, DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, I2C_HandleTypeDef *hi2c)
{  // , std::function<void (UART_HandleTypeDef *, string)> uart_TX_IT   not needed
	extern int BIOZAP_Sample_Lgth;

	paramstruc param;
	size_t from = 0;
	size_t to = 0;
	if ( (comm_Str.find(LF, from) == string::npos) || (comm_Str[comm_Str.length()-1] != LF)) {
		KiIratas(hi2c, huart, "Missing LF in str!" + EOL, true);
		return;
	}
	while( (to = comm_Str.find(LF, from)+1) >= from ) {  // Get a Command String with LF ending.
		string one_Str = comm_Str.substr(from, to-from); // from a string or a program.
		from = to;
		if (abort_Prog_Run) {
			KiIratas(hi2c, huart, "Aborting !" + EOL, false);
			abort_Prog_Run = false;
			beep(hi2c, huart, 500);
			return;
		}
		findAndReplaceAll(one_Str, "\r", "");  // Delete all CR in the inp_Str
		findAndReplaceAll(one_Str, "\n", EOL); // Change all LF to EOL in the inp_Str
		KiIratas(hi2c, huart, one_Str, true);
		if (!getParams(one_Str, &param, huart, hi2c)){ // set param[x]'s.
			KiIratas(hi2c, huart, "Syntax error !" + EOL, true);
			beep(hi2c, huart, 500);
			//return;
		}
		else if(param.command.length() == 0){  // Have to checked this, later param[0].at(0) chashed an empty string
			KiIratas(hi2c, huart, "Empty command !" + EOL, true);
		}
		else if (store_Program) {
			if(param.command != "@") {
				user_Program += one_Str;
			//	KiIratas(hi2c, huart, one_Str, true);
			}
			else {
				store_Program = false;
				KiIratas(hi2c, huart, "Program stored !" + EOL, true);
				save_string_to_flash(user_Program);
			}	
		}
		else if (param.command == "freq") {
			//double freq = std::stod(param.param[1]);
			freq_item element = find_time_freq(param.paramNum[0]);
			if(element.error < 1.0){
				KiIratas(hi2c, huart, one_Str, true);
				start_DMA(hdac, htim, element.psc , element.arr - 1);
				string numstr = to_string((uint32_t)element.freq);
				string precstr = to_string((uint32_t)((element.freq - (uint32_t)element.freq) * 100));
				KiIratas(hi2c, huart, "freq:" + numstr + "." + precstr + " Smpl:" + to_string(BIOZAP_Sample_Lgth) + " arr:" + to_string(element.arr - 1) + " working ...  " + EOL, false);
				//Delay(huart, hdac, std::stol(param.param[2]));
				Delay(hi2c, huart, param.paramNum[1]*1000);
				stop_DMA(hdac, htim);
				KiIratas(hi2c, huart, "Ok." + EOL, true);
			}
			else{
				KiIratas(hi2c, huart, "freq error :" + to_string((int)element.error) + EOL, true);
			}
		}
		else if (param.command == "exe") {
			//int e_idx = std::stoi(param.param[1]);
			int e_idx = param.paramNum[0];
			if (e_idx > 0 && e_idx < 10) {
				Command_Interpreter(internalProgram[e_idx].item, huart, hdac, htim, hi2c);
			}
			else if (e_idx == 0) {
				Command_Interpreter(user_Program, huart, hdac, htim, hi2c);
			}
			else {
				KiIratas(hi2c, huart, "Param[1] must have in 0..9" + EOL, true);
			}
		
		}
		else if (param.command == "ls") {
			findAndReplaceAll(user_Program, "\r", "");	// Delete all CR in the inp_Str
			findAndReplaceAll(user_Program, "\n", EOL);	// Change all LF to EOL in the inp_Str
			KiIratas(hi2c, huart, user_Program, true);
		}
		else if (param.command == "mem") {
			user_Program = "";
			store_Program = true;
			KiIratas(hi2c, huart, "Ok." + EOL, true);
		}
		else if (param.command == "wait") {
			//HAL_Delay(std::stol(param.param[1]));
			//HAL_Delay(param.param[0]);
			KiIratas(hi2c, huart, one_Str, true);
			Delay(hi2c, huart, param.paramNum[0]);
			KiIratas(hi2c, huart, "Ok." + EOL, true);
		}
		else if (param.command == "off") {
			KiIratas(hi2c, huart, "Ok." + EOL, true);
		}
		else if (param.command == "beep") {
			if (param.paramNum[0] == 0)
				param.paramNum[0] = 100;
			//beep(std::stol(param.param[1]));
			beep(hi2c, huart, param.paramNum[0]);
			KiIratas(hi2c, huart, "Ok." + EOL, true);
		}
		else if (param.command == "vout") {
			if (param.paramNum[0] == 0)
				param.paramNum[0] = vout;
			vout = param.paramNum[0];
			KiIratas(hi2c, huart, param.paramStr[0] + EOL + "Ok." + EOL, true);
		}
		else if (param.command == "vmin") {
			if (param.paramNum[0] == 0)
				param.paramNum[0] = vmin;
			vmin = param.paramNum[0];
			KiIratas(hi2c, huart, param.paramStr[0] + EOL + "Ok." + EOL, true);
		}
		else if (param.command == "pbar") {
			KiIratas(hi2c, huart, "Ok." + EOL, true);
		}
		else if (param.command.at(0) == '#') { // comment
			;
		}
		else if (param.command.at(0) == ':') { // Label
			;
		}
		else if (param.command.at(0) == '@') { // mem command over 
			;
		}
		else if (param.command == "abort") {
			KiIratas(hi2c, huart, "Ok." + EOL, true);
		}
		else if (param.command == "Eszti")
		{
			KiIratas(hi2c, huart, "Szeli." + EOL, true);
		}
		else if (param.command == "chp")
		{
			KiIratas(hi2c, huart, "Ok." + EOL, true);
		}
		else if (param.command == "signal")
		{
			signal_form = param.paramNum[0];
			if(signal_form < 0 || signal_form > 3)
				signal_form = BIOZAP_SIN;
			KiIratas(hi2c, huart, "Signal : " + to_string(signal_form) + EOL, true);
		}
		else
		{
			KiIratas(hi2c, huart, "unknown command -->" + param.command + "<--" + EOL, true);
		}
	}
//	if (param.command != "exe")  // exe command recursive call this function
//		KiIratas(hi2c, huart, ">", true);
}

void Cmd_Interpreter(UART_HandleTypeDef *huart, DAC_HandleTypeDef *hdac, TIM_HandleTypeDef *htim, I2C_HandleTypeDef *hi2c)
{
	if (huart->Instance == USART2) {
		while (command_Line.size() >0)
		{
			Command_Interpreter(command_Line.front(), huart, hdac, htim, hi2c);
			command_Line.pop_front();
		}
	}
	if (huart->Instance == USART1) {
		while (command_Line1.size() > 0)
		{
			Command_Interpreter(command_Line1.front(), huart, hdac, htim, hi2c);
			command_Line1.pop_front();
		}
	}
}
