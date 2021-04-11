/*
 * multiZAP_programs.h
 *
 *  Created on: 23 Jan 2017
 *
 *      Therapy scripts developed by:
 *      PhD Nenah Sylver
 *      The Rife Handbook 2011
 *      ISBN 9780981807515
 *
 */

#ifndef MULTIZAP_PROGRAMS_H_
#define MULTIZAP_PROGRAMS_H_

#include <string>
#include <list>

using namespace std;


struct storedprogram{
    std::string item;
};


void read_flash(string *data);
void save_to_flash(storedprogram data);
void save_string_to_flash(string data);

const storedprogram internalProgram[] = {
//const list<string> internalProgram[] = {
		{":0\n"},
		{":1\n"
		 "#Chlamydia 36m\n"
		 "wait 3000\n"
		 "pbar 100 2160\n"
		 "beep 100\n"
		 "freq 471050 180\n"
		 "freq 47100 60\n"
		 "freq 47150 60\n"
		 "freq 47200 60\n"
		 "freq 47900 180\n"
		 "freq 62000 180\n"
		 "freq 94010 180\n"
		 "freq 94290 180\n"
		 "freq 188010 180\n"
		 "freq 188590 180\n"
		 "freq 376030 180\n"
		 "freq 377170 180\n"
		 "freq 752050 180\n"
		 "freq 754340 180\n"
		 "beep 500\n"
		 "off\n"},
		{":2\n"
		 "#Candida 21m\n"
		 "wait 3000\n"
		 "pbar 100 1260\n"
		 "beep 100\n"
		 "freq 2048500 180\n"
		 "freq 5115500 180\n"
		 "freq 5115600 180\n"
		 "freq 5394000 180\n"
		 "freq 5891400 180\n"
		 "freq 5891600 180\n"
		 "freq 8874000 180\n"
		 "beep 500\n"
		 "off\n"

		},
		{":3\n"
		 "#Mycoplasma 27m\n"
		 "wait 3000\n"
		 "pbar 100 1620\n"
		 "beep 100\n"
		 "freq 66000 180\n"
		 "freq 69000 180\n"
		 "freq 72750 180\n"
		 "freq 68800 180\n"
		 "freq 70920 180\n"
		 "freq 77700 180\n"
		 "freq 97500 180\n"
		 "freq 268800 180\n"
		 "freq 283850 180\n"
		 "beep 500\n"
		 "off\n"},
		{":4\n"
		 "#1 Borrelia 30m\n"
		 "wait 3000\n"
		 "pbar 100 1800\n"
		 "beep 100\n"
		 "freq 3408600 180\n"
		 "freq 4105500 180\n"
		 "freq 5505500 180\n"
		 "freq 5506200 180\n"
		 "freq 5844300 180\n"
		 "freq 7291200 180\n"
		 "freq 7452000 180\n"
		 "freq 7514100 180\n"
		 "freq 7886400 180\n"
		 "freq 8694000 180\n"
		 "beep 500\n"
		 "off\n"},
		{":5\n"
		 "#2 Borrelia 72m\n"
		 "wait 3000\n"
		 "pbar 100 4320\n"
		 "beep 100\n"
		 "freq 2430400 180\n"
		 "freq 2628800 180\n"
		 "freq 3408600 180\n"
		 "freq 3422400 180\n"
		 "freq 3811500 180\n"
		 "freq 4155200 180\n"
		 "freq 5402700 180\n"
		 "freq 5409600 180\n"
		 "freq 5423400 180\n"
		 "freq 5330300 180\n"
		 "freq 5505500 180\n"
		 "freq 5681000 180\n"
		 "freq 5704000 180\n"
		 "freq 5830500 180\n"
		 "freq 5837400 180\n"
		 "freq 5844300 180\n"
		 "freq 5851200 180\n"
		 "freq 7291200 180\n"
		 "freq 7771500 180\n"
		 "freq 7886400 180\n"
		 "freq 7953400 180\n"
		 "freq 7969500 180\n"
		 "freq 7985600 180\n"
		 "freq 8365500 180\n"
		 "beep 500\n"
		 "off\n"},
		{":6\n"
		 "#3 Borrelia 2h\n"
		 "wait 3000\n"
		 "pbar 100 7020\n"
		 "beep 100\n"
		 "freq 2484000 180\n"
		 "freq 2504700 180\n"
		 "freq 2898000 180\n"
		 "freq 2909500 180\n"
		 "freq 3595500 180\n"
		 "freq 4089400 180\n"
		 "freq 4105500 180\n"
		 "freq 4140000 180\n"
		 "freq 4174500 180\n"
		 "freq 5009400 180\n"
		 "freq 5014800 180\n"
		 "freq 5016300 180\n"
		 "freq 5023200 180\n"
		 "freq 5216400 180\n"
		 "freq 5237100 180\n"
		 "freq 5257800 180\n"
		 "freq 5278500 180\n"
		 "freq 5492400 180\n"
		 "freq 5499300 180\n"
		 "freq 5506200 180\n"
		 "freq 5513100 180\n"
		 "freq 5992500 180\n"
		 "freq 6375600 180\n"
		 "freq 6400900 180\n"
		 "freq 6426200 180\n"
		 "freq 6451500 180\n"
		 "freq 6502500 180\n"
		 "freq 7452000 180\n"
		 "freq 7514100 180\n"
		 "freq 7534800 180\n"
		 "freq 7564700 180\n"
		 "freq 7594600 180\n"
		 "freq 7624500 180\n"
		 "freq 8349000 180\n"
		 "freq 8389500 180\n"
		 "freq 8360500 180\n"
		 "freq 8694000 180\n"
		 "freq 8763000 180\n"
		 "freq 8797500 180\n"
		 "beep 500\n"
		 "off\n"},
		{":7\n"
		 "#4 Borrelia 72m\n"
		 "wait 3000\n"
		 "pbar 100 4320\n"
		 "beep 100\n"
		 "freq 3408600 180\n"
		 "freq 4105500 180\n"
		 "freq 4112400 180\n"
		 "freq 4119300 180\n"
		 "freq 4126200 180\n"
		 "freq 4297800 180\n"
		 "freq 5188800 180\n"
		 "freq 5195700 180\n"
		 "freq 5202600 180\n"
		 "freq 5209500 180\n"
		 "freq 5796000 180\n"
		 "freq 5809800 180\n"
		 "freq 5816700 180\n"
		 "freq 5823600 180\n"
		 "freq 6842500 180\n"
		 "freq 6854000 180\n"
		 "freq 6865500 180\n"
		 "freq 6877000 180\n"
		 "freq 8625000 180\n"
		 "freq 8636500 180\n"
		 "freq 8648000 180\n"
		 "freq 8659500 180\n"
		 "freq 8692500 180\n"
		 "freq 8984500 180\n"
		 "beep 500\n"
		 "off\n"},
		{":8\n"
		 "#Earth rhythm 8m\n"
		 "wait 3000\n"
		 "beep 100\n"
		 "freq 783 480\n"
		 "beep 500\n"
		 "off\n"},
		{":9\n"
		 "#Pineal gland 16m\n"
		 "wait 3000\n"
		 "pbar 100 960\n"
		 "beep 100\n"
		 "freq 783 120\n"
		 "freq 2000 180\n"
		 "freq 53700 180\n"
		 "freq 66200 180\n"
		 "freq 93600 300\n"
		 "beep 500\n"
		 "off\n"},
		{"@"}};

#endif /* MULTIZAP_PROGRAMS_H_ */
