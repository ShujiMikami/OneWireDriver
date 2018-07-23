/*
 * DS18B20Driver.c
 *
 *  Created on: 2018/07/22
 *      Author: Shuji
 */
#include "DS18B20Driver.h"
#include "OneWireDriver.h"

#define CODE_CONVERT (unsigned char)0x44
#define CONVERSION_BUISY (unsigned char)0
#define CONVERSION_FINISH (unsigned char)1

static void waitForConvert();

void Convert()
{
	WriteByte(CODE_CONVERT);
}
void waitForConvert()
{
	while(1){
		unsigned char status = ReadSlot();
		if(status == CONVERSION_FINISH){
			break;
		}
	}
}
void WriteScratchPad()
{

}
void ReadScratchPad()
{

}
void RecallE2()
{

}
void ReadPowerSupply()
{

}
