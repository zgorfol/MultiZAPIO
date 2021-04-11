/*
 * biozap_freq.h
 *
 *  Created on: 30.12.2018
 *      Author: kczoba
 */

#ifndef BIOZAP_FREQ_H_
#define BIOZAP_FREQ_H_

#define BIOZAP_SAMPLE_SIZE 2048 // 12bit sample (0-4095) size

//WavesTypes
#define BIOZAP_USR 0 // User defined wave by the sample
#define BIOZAP_SIN 1 // Sine wave (default)
#define BIOZAP_REC 2 // Square/rectangular wave
#define BIOZAP_SAW 3 // Saw triangle wave

struct freq_item{
	uint16_t psc;
	uint16_t arr;
	double error;
	double freq;
};

freq_item find_time_freq(uint32_t freq);
// void set_time_freq(TIM_HandleTypeDef *htim, uint16_t psc, uint16_t arr);
uint8_t generate_sample(uint16_t vmin, uint16_t vampl, uint8_t waveType, uint16_t *sample_array);

#endif /* BIOZAP_FREQ_H_ */

