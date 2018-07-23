/*
 * DS18B20Driver.c
 *
 *  Created on: 2018/07/22
 *      Author: Shuji
 */
#include "DS18B20Driver.h"
#include "OneWireDriver.h"

#define SCRATCH_PAD_BYTE_SIZE 9

#define CODE_CONVERT (unsigned char)0x44
#define CONVERSION_BUISY (unsigned char)0
#define CONVERSION_FINISH (unsigned char)1

#define CODE_READ_SCRATCHPAD (unsigned char)0xBE
#define TEMP_LSB_POS 0
#define TEMP_MSB_POS 1
#define T_H_POS 2
#define T_L_POS 3
#define USER_BYTE_1_POS T_H_POS
#define USER_BYTE_2_POS T_L_POS
#define CONFIGURATION_REG_POS 4
#define CRC_POS 8

#define CODE_WRITE_SCRATCHPAD (unsigned char)0x4E
#define WRITABLE_BYTE_LENGTH 3

static void waitForConvert();

void Convert()
{
	WriteByte(CODE_CONVERT);

	waitForConvert();
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
void WriteScratchPad(ScratchPadData_t dataToWrite)
{
	unsigned char data[WRITABLE_BYTE_LENGTH] = {0};

	data[0] = dataToWrite.T_H_Register;
	data[1] = dataToWrite.T_L_Register;
	data[2] = dataToWrite.Configuration_Register;

	WriteByte(CODE_WRITE_SCRATCHPAD);
	int cnt = 0;
	for(cnt = 0; cnt < WRITABLE_BYTE_LENGTH; cnt++){
		WriteByte(data[cnt]);
	}
}
ScratchPadData_t ReadScratchPad()
{
	ScratchPadData_t result;

	WriteByte(CODE_READ_SCRATCHPAD);

	int cnt = 0;
	unsigned char data[SCRATCH_PAD_BYTE_SIZE] = {0};
	for(cnt = 0; cnt < SCRATCH_PAD_BYTE_SIZE; cnt++){
		data[cnt] = ReadByte();
	}

	result.Temperature = (unsigned short)data[TEMP_LSB_POS] | ((unsigned short)data[TEMP_MSB_POS] << 8);
	result.T_L_Register = data[T_L_POS];
	result.T_H_Register = data[T_H_POS];
	result.Configuration_Register = data[CONFIGURATION_REG_POS];
	result.CRC_Data = data[CRC_POS];

	return result;
}
void RecallE2()
{

}
void ReadPowerSupply()
{

}
