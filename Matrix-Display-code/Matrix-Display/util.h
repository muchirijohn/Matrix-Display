/*
* util.h
*
* Created: 6/16/2013 07:57:58
* (c) Muchiri John
*/


#ifndef UTIL_H_
#define UTIL_H_

#include "consts.h"

uint8_t updatedBufferArray[MT_SIZE];
uint8_t currentArray[MT_SIZE];
/************************************************************************/
/* initialize the buffers                                               */
/************************************************************************/
void InitializeBuffers(){
	uint8_t len = 0;
	for(len=0;len<MT_SIZE;len++){
		updatedBufferArray[len] = 0;
		currentArray[len] = 0;
	}
}

/************************************************************************/
/* fill current array with previous data                                */
/************************************************************************/
void FillCurrentArray()
{
	uint8_t len = 0;
	for(len=0; len<MT_SIZE;len++) currentArray[len] = updatedBufferArray[len];
}

/************************************************************************/
/* shift array to right and insert data                                 */
/************************************************************************/
uint8_t *ArrayShiftRightInsert(const uint8_t data)
{
	uint8_t index = MT_SIZE -1;
	FillCurrentArray();
	updatedBufferArray[index--] = data;
	for(; index>=0; index--){
		updatedBufferArray[index] = currentArray[index+1];
		if(index==0) break;
	}
	//updatedBufferArray[0] = currentArray[1];
	return updatedBufferArray;
}


#endif /* UTIL_H_ */