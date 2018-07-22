/*
 * OneWireDriver.h
 *
 *  Created on: 2018/07/13
 *      Author: Shuji
 */

#ifndef ONEWIREDRIVER_INC_ONEWIREDRIVER_H_
#define ONEWIREDRIVER_INC_ONEWIREDRIVER_H_




typedef enum{
	ONE_WIRE_SUCCESS,
	ONE_WIRE_FAIL
}ONE_WIRE_STATUS_t;

typedef struct{
	unsigned char Family_Code;
	unsigned short SerialNumber_H;
	unsigned short SerialNumber_M;
	unsigned short SerialNumber_L;
	unsigned char CRC_Code;
}ONE_WIRE_ROM_CODE_t;

void ResetPulse();
ONE_WIRE_STATUS_t ReadPresensePulse();
void WriteByte(unsigned char byteData);
unsigned char ReadByte();
void SearchRom();
ONE_WIRE_ROM_CODE_t ReadRom();
void MatchRom();
void SkipRom();


#endif /* ONEWIREDRIVER_INC_ONEWIREDRIVER_H_ */
