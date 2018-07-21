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

void ResetPulse();
ONE_WIRE_STATUS_t ReadPresensePulse();
void WriteByte(unsigned char byteData);
unsigned char ReadByte();



#endif /* ONEWIREDRIVER_INC_ONEWIREDRIVER_H_ */
