/*
 * OneWireDriver.c
 *
 *  Created on: 2018/07/13
 *      Author: Shuji
 */
#include "OneWireDriver.h"

#include "OneWireSpecifications.h"
#include "GPIO_Wrapper.h"
#include "Wait_Wrapper.h"

void ResetPulse()
{
	//GPIOをTxモードに
	SetPin2TxMode();

	//GPIOをアサート
	AssertPin();

	//tRSTHだけ待つ
	Wait_us(T_RSTH);

	//GPIOをネゲート
	NegatePin();
}
ONE_WIRE_STATUS_t ReadPresensePulse()
{
	ONE_WIRE_STATUS_t result = ONE_WIRE_FAIL;

	//TODO ReadPresensePulse実装

	return result;
}
