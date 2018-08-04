/*
 * DS18B20Driver.h
 *
 *  Created on: 2018/07/22
 *      Author: Shuji
 */

#ifndef DS18B20DRIVER_INC_DS18B20DRIVER_H_
#define DS18B20DRIVER_INC_DS18B20DRIVER_H_

#include "OneWireDriver.h"

typedef struct{
	unsigned short Temperature;
	unsigned char T_H_Register;
	unsigned char T_L_Register;
	unsigned char Configuration_Register;
	unsigned char CRC_Data;
}ScratchPadData_t;

typedef enum{
	RESOLUTION_9BIT = 0,
	RESOLUTION_10BIT = 1,
	RESOLUTION_11BIT = 2,
	RESOLUTION_12BIT = 3
}RESOLUTION_t;

ONE_WIRE_STATUS_t Convert(ONE_WIRE_ROM_CODE_t romCode);
ONE_WIRE_STATUS_t SetResolution_t(ONE_WIRE_ROM_CODE_t romCode, RESOLUTION_t resolution);
void WriteScratchPad(ONE_WIRE_ROM_CODE_t romCode, ScratchPadData_t dataToWrite);
ScratchPadData_t ReadScratchPad(ONE_WIRE_ROM_CODE_t romCode);
void RecallE2(ONE_WIRE_ROM_CODE_t romCode);
void ReadPowerSupply(ONE_WIRE_ROM_CODE_t romCode);

#endif /* DS18B20DRIVER_INC_DS18B20DRIVER_H_ */
