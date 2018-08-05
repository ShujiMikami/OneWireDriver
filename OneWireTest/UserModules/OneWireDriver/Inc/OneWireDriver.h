/*
 * OneWireDriver.h
 *
 *  Created on: 2018/07/13
 *      Author: Shuji
 */

#ifndef ONEWIREDRIVER_INC_ONEWIREDRIVER_H_
#define ONEWIREDRIVER_INC_ONEWIREDRIVER_H_

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;


typedef enum{
	ONE_WIRE_SUCCESS,
	ONE_WIRE_FAIL
}ONE_WIRE_STATUS_t;

typedef struct{
	uint8_t Family_Code;
	uint16_t SerialNumber_H;
	uint16_t SerialNumber_M;
	uint16_t SerialNumber_L;
	uint8_t CRC_Code;
}ONE_WIRE_ROM_CODE_t;

ONE_WIRE_STATUS_t ResetPulse();
void WriteByte(unsigned char byteData);
uint8_t ReadByte();
void SearchRom();
ONE_WIRE_ROM_CODE_t ReadRom();
void MatchRom(ONE_WIRE_ROM_CODE_t romCodeToMatch);
void SkipRom();
uint8_t ReadSlot();
uint8_t GetNumOfROMCodeFound();
void GetROMCode(ONE_WIRE_ROM_CODE_t* buffer, uint8_t count);


#endif /* ONEWIREDRIVER_INC_ONEWIREDRIVER_H_ */
