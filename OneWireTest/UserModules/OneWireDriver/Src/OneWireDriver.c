/*
 * OneWireDriver.c
 *
 *  Created on: 2018/07/13
 *      Author: Shuji
 */

#define BYTE_SIZE 8

#include "OneWireDriver.h"

#include "OneWireSpecifications.h"
#include "GPIO_Wrapper.h"
#include "Wait_Wrapper.h"

static void write0();
static void write1();
static unsigned char readBit();

void ResetPulse()
{
	//GPIOをTxモードに
	SetPin2TxMode();

	//GPIOをアサート
	AssertPin();

	//tRSTLだけ待つ
	Wait_us(T_RSTL);

	//GPIOをネゲート
	NegatePin();
}
ONE_WIRE_STATUS_t ReadPresensePulse()
{
	ONE_WIRE_STATUS_t result = ONE_WIRE_FAIL;

	//GPIOをRxモードに
	SetPin2RxMode();

	//GPIOの状態を監視
	int cnt = 0;
	GPIO_STATE_t currentStatus = GPIO_NEGATED;
	int startCnt = -1;//最初からAssertの可能性もあるので-1にしとく
	int endCnt = -1;
	for(cnt = 0; cnt < T_RSTH; cnt++){//T_RSTHの間,計測し続ける
		//ピンの状態を取得
		currentStatus = GetPinState();

		if(startCnt < 0){//初回のアサート未検出
			if(currentStatus == GPIO_ASSERTED){//アサートならば
				startCnt = cnt;
			}
		}
		else{//アサート検出済み
			if(currentStatus == GPIO_NEGATED && endCnt < 0){//ネゲート未検出
				endCnt = cnt;
			}
		}
		Wait_us(1);//1usec周期
	}

	if(endCnt - startCnt >= T_PDLOW_MIN && endCnt - startCnt <= T_PDLOW_MAX){//規定の範囲のアサートパルスが着たら
		result = ONE_WIRE_SUCCESS;
	}
	else{
		result = ONE_WIRE_FAIL;
	}


	return result;
}
void write0()
{
	//GPIOをTxモードに
	SetPin2TxMode();

	//GPIOをアサート
	AssertPin();

	//tRSTLだけ待つ
	Wait_us(T_LOW0);

	//GPIOをネゲート
	NegatePin();

	//time Slot単位待つ
	Wait_us(T_SLOT - T_LOW0);

	//Recovery
	Wait_us(T_REC);
}
void write1()
{
	//GPIOをTxモードに
	SetPin2TxMode();

	//GPIOをアサート
	AssertPin();

	//tRSTLだけ待つ
	Wait_us(T_LOW1);

	//GPIOをネゲート
	NegatePin();

	//time Slot単位待つ
	Wait_us(T_SLOT - T_LOW1);

	//Recovery
	Wait_us(T_REC);
}
void WriteByte(unsigned char byteData)
{
	int cnt = 0;
	for(cnt = 0; cnt < BYTE_SIZE; cnt++){
		if(((byteData >> cnt) & 0x01) == 0x01){
			write1();
		}
		else{
			write0();
		}
	}
}
unsigned char readBit()
{
	unsigned char result = 0;

	//GPIOをTxモードに
	SetPin2TxMode();

	//GPIOをアサート
	AssertPin();

	//t_INTだけ待つ
	Wait_us(T_INT);

	//GPIOをネゲート
	NegatePin();

	//GPIOをRxモードに
	SetPin2RxMode();

	//T_RCだけ待つ
	Wait_us(T_RC);

	//状態取得
	GPIO_STATE_t currentGPIOState = GetPinState();

	//T_Slotまで待つ
	Wait_us(T_SLOT - T_RC - T_INT);

	//Recovery
	Wait_us(T_REC);

	//ビットに反映
	if(currentGPIOState == GPIO_ASSERTED){
		result = 0;
	}
	else{
		result = 1;
	}

	return result;
}
unsigned char ReadByte()
{
	unsigned char result = 0;

	int cnt = 0;
	for(cnt = 0; cnt < BYTE_SIZE; cnt++){
		unsigned char currentBit = readBit();
		result |= (currentBit << cnt);
	}

	return result;
}
