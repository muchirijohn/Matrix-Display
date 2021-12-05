/*
* util.h
*
* Created: 6/16/2013 07:42:MT_SIZE
* (c) Muchiri John
*
* Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in
the documentation and/or other materials provided with the
distribution.

* Neither the name of the copyright holders nor the names of
contributors may be used to endorse or promote products derived
from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE. See the GNU General Public License for more details.
*/



#ifndef SHIFT_EFFECTS_H_
#define SHIFT_EFFECTS_H_

#include <avr/pgmspace.h>
/************************************************************************/
/* Frequency 8MHz                                                      */
/************************************************************************/
#define  F_CPU 16000000L
#include <util/delay.h>
#include "pin.h"
#include "data.h"
#include "font_5x7.h"
#include "util.h"
/************************************************************************/
/* Shift register pins													*/
/************************************************************************/
#define data_pin 3
#define latch_pin 4
#define clock_pin 5
/************************************************************************/
/* global index for selecting multiplexing pins.                        */
/************************************************************************/
uint8_t plex_index = 0;
/************************************************************************/
/* pins for multiplexing.                                               */
/************************************************************************/
multiPins plex_pins[7] = {12, 13, 14, 15, 16, 17, 18};
/************************************************************************/
/* buffer to hold data for display during effects		                */
/************************************************************************/
uint8_t buffer[MT_SIZE];
uint8_t *bf = buffer;
/************************************************************************/
/* Arrays data.															*/
/************************************************************************/
extern const uint8_t font_5x7[];
/************************************************************************/
/*  delay                                                               */
/************************************************************************/
int delayms = 1;
static inline void delay(int dl)
{
	while(dl >= 0)
	{
		_delay_ms(1);
		dl--;
	}
}
/************************************************************************/
/* clear buffer                                                         */
/************************************************************************/
void clearBuffer(){
	uint8_t len = 0;
	for(;len < MT_SIZE; len++){
		*(bf+len) = 0;
	}
}
/************************************************************************/
/* Multiplexing method                                                  */
/************************************************************************/
static inline void MultiPlex(uint8_t *pin)
{
	pinsMultiWrite(plex_pins, LOW);
	pinWrite(plex_pins[*pin], HIGH);
	delay(delayms);
}

/************************************************************************/
/* Clear Display                                                        */
/************************************************************************/
static inline void Clear(){
	int x, y = 0;
	pinsMultiWrite(plex_pins, LOW);
	for(x = 0; x < (SH_REG + 1); x++) {
		pinWrite(latch_pin, LOW);
		for(y = 0; y<= FONT_HEIGHT ; y++){
			pinWrite(data_pin, (0 & (1  << (FONT_HEIGHT - y))));
			//toggle the clock pin
			pinWrite(clock_pin, HIGH);
			pinWrite(clock_pin, LOW);
		}
		pinWrite(latch_pin, HIGH);
	}
}

/************************************************************************/
/* Display data                                                         */
/************************************************************************/
static inline void DisplayBuffer(const uint8_t *p)
{
	uint8_t byte, index, bit = 0;
	uint8_t pos = MT_SIZE - 1;
	uint8_t pos_sz = pos;
	//output byte
	for(byte=0; byte<FONT_HEIGHT; byte++){
		pos = pos_sz;
		for(index=0; index< SH_REG; index++){
			pinWrite(latch_pin, LOW);
			for(bit = 0; bit <= FONT_HEIGHT ; bit++){
				pinWrite(data_pin,((*(p+pos)) & (1 << byte)));
				//toggle the clock pin
				pinWrite(clock_pin, HIGH);
				pinWrite(clock_pin, LOW);
				pos--;
				if(pos < 0) break;
			}
			pinWrite(latch_pin, HIGH);
		}
		MultiPlex(&byte);
		Clear();
	}
	//delayms = 1;
}
/************************************************************************/
/* initialize                                                           */
/************************************************************************/
void init()
{
	pinSet(data_pin, OUTPUT); /* DS		data pin*/
	pinSet(latch_pin, OUTPUT); /* ST_CP     latch pin*/
	pinSet(clock_pin, OUTPUT); /* SH_HP     clock pin*/
	pinsMultiSet(plex_pins, OUTPUT); /* multiplexing pins*/
}

/************************************************************************/
/* Hold display for specified time                                      */
/************************************************************************/
/*
static inline void HoldDisplay(const uint8_t *data, uint8_t period)
{
uint8_t x = 0;
for(x=0; x< period; x++){
Display(data);
}
}*/
static inline void HoldDisplayBuffer(const uint8_t *data, uint8_t period)
{
	uint8_t x = 0;
	for(x=0; x< period; x++){
		DisplayBuffer(data);
	}
}
/************************************************************************/
/*split while displaying                                                */
/************************************************************************/
void Rain(const uint8_t *p, uint8_t delay)
{
	uint8_t rt = 0;
	delayms = delay;
	for(rt=0; rt<100; rt++){
		DisplayBuffer(p);
	}
}
/************************************************************************/
/* scroll right                                                         */
/************************************************************************/
void ScrollRight(const uint8_t *p, uint8_t time)
{
	uint8_t index, begin, end = 0;
	//*bf = buffer;
	for(index = 0; index < MT_SIZE; index++)
	{
		for(begin = 0; begin <= index; begin++) (*(bf + begin) = 0x00);
		for(end = index; end < MT_SIZE; end++) (*(bf + end) = (*((p+(end-index)))));
		HoldDisplayBuffer(buffer, time);
	}
}

/************************************************************************/
/* delete right                                                         */
/************************************************************************/
void DeleteRight(const uint8_t *p, uint8_t time)
{
	uint8_t index, begin, end = 0;
	//uint8_t *bf = buffer;
	for(index = 0; index < MT_SIZE; index++)
	{
		for(begin = 0; begin <= index; begin++) (*(bf + begin) = 0x00);
		for(end = index; end < MT_SIZE; end++) (*(bf + end) = (*((p+(end)))));
		HoldDisplayBuffer(buffer, time);
	}
}
/************************************************************************/
/* scroll left                                                        */
/************************************************************************/
void ScrollLeft(const uint8_t *p, uint8_t time)
{
	uint8_t index, begin, end = 0;
	//uint8_t *bf = buffer;
	for(index = MT_SIZE-1 ; index > 0; index--)
	{
		for(begin = 0; begin <= index; begin++) (*(bf + begin)) = (*((p+(begin+MT_SIZE-index))));
		for(end = (index); end < MT_SIZE ; end++) (*(bf + end)) = 0x00;
		HoldDisplayBuffer(buffer, time);
	}
	//HoldDisplayBuffer(buffer, time);
}

/************************************************************************/
/* Type in		                                                       */
/************************************************************************/
void TypeIn(const uint8_t *p, uint8_t time)
{
	uint8_t index, begin, end = 0;
	//uint8_t *bf = buffer;
	//HoldDisplay(p, 10);
	for(index = 0; index < MT_SIZE; index++)
	{
		for(begin = 0; begin <= index; begin++)(*(bf + begin) = (*((p+(begin)))));
		for(end = index; end < MT_SIZE; end++) (*(bf + end) = 0x00);
		HoldDisplayBuffer(buffer, time);
	}
}
/************************************************************************/
/* Delete left		                                                       */
/************************************************************************/
void DeleteLeft(const uint8_t *p, uint8_t time)
{
	uint8_t index, begin, end = 0;
	//uint8_t *bf = buffer;
	for(index = MT_SIZE -1 ; index > 0; index--)
	{
		for(begin = 0; begin <= index; begin++) (*(bf + begin)) = (*((p+begin)));
		for(end = (index-1); end < MT_SIZE ; end++) (*(bf + end)) = 0x00;
		HoldDisplayBuffer(buffer, time);
	}
	HoldDisplayBuffer(buffer, time);
}
/************************************************************************/
/* drop effect                                                     */
/************************************************************************/
enum direction{ up, down};
void Drop(const uint8_t *p, uint8_t time, uint8_t dir )
{
	//uint8_t *bf = buffer;
	uint8_t index, step = 0;
	for(step = 0; step < FONT_HEIGHT; step++)
	{
		for(index = 0; index < MT_SIZE; index++)
		{
			if(dir)
			*(bf + index) = (*((p+index)) >> (FONT_HEIGHT-step));
			else
			*(bf + index) = (*((p+index)) << (FONT_HEIGHT-step));
		}
		HoldDisplayBuffer(bf, time);
	}
	HoldDisplayBuffer(p,time);
}
/************************************************************************/
/* fall effect                                                     */
/************************************************************************/
void Fall(const uint8_t *p, uint8_t time, uint8_t dir )
{
	//uint8_t *bf = buffer;
	uint8_t index, step = 0;
	for(step = 0; step <= FONT_HEIGHT; step++)
	{
		for(index = 0; index < MT_SIZE; index++)
		{
			if(!dir)
			*(bf + index) = (*((p+index)) >> (step));
			else
			*(bf + index) = (*((p+index)) << (step));
		}
		HoldDisplayBuffer(bf, time);
	}
	HoldDisplayBuffer(bf,50);
}
/************************************************************************/
/* blinker                                                         */
/************************************************************************/
void Blinker(const uint8_t *p, uint8_t time, uint8_t times)
{
	while(times-- > 0)
	{
		HoldDisplayBuffer(p, time);
		Clear();
		_delay_ms(500);
	}
}

/************************************************************************/
/* scroll long letters                                                  */
/************************************************************************/
void ScrollLongLeft(const char *ch, uint8_t time)
{
	uint8_t *chbuffer;// = CharBytes(ch++);
	InitializeBuffers();
	int len = 0;
	while(*ch){
		chbuffer = CharBytes(ch++);
		for(len =0; len<FONT_WIDTH; len++){
			HoldDisplayBuffer(ArrayShiftRightInsert(*(chbuffer+len)), time);
		}
		HoldDisplayBuffer(ArrayShiftRightInsert(0), time);
	}
	for(len =0; len<MT_SIZE; len++){
		HoldDisplayBuffer(ArrayShiftRightInsert(0), time);
	}
}
#endif /* SHIFT_EFFECTS_H_ */