/*
 * DS18B20Driver.h
 *
 *  Created on: 2018/07/22
 *      Author: Shuji
 */

#ifndef DS18B20DRIVER_INC_DS18B20DRIVER_H_
#define DS18B20DRIVER_INC_DS18B20DRIVER_H_

typedef struct{
	unsigned short Temperature;
	unsigned char T_H_Register;
	unsigned char T_L_Register;
	unsigned char Configuration_Register;
	unsigned char CRC_Data;
}ScratchPadData_t;

void Convert();
void WriteScratchPad(ScratchPadData_t dataToWrite);
ScratchPadData_t ReadScratchPad();
void RecallE2();
void ReadPowerSupply();

#endif /* DS18B20DRIVER_INC_DS18B20DRIVER_H_ */
