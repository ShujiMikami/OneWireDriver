/*
 * OneWireDriver.c
 *
 *  Created on: 2018/07/13
 *      Author: Shuji
 */

#define BYTE_SIZE 8
#define FAMILY_CODE_POS 0
#define SERIAL_NUMBER_LL_POS 1
#define SERIAL_NUMBER_LH_POS 2
#define SERIAL_NUMBER_ML_POS 3
#define SERIAL_NUMBER_MH_POS 4
#define SERIAL_NUMBER_HL_POS 5
#define SERIAL_NUMBER_HH_POS 6
#define CRC_CODE_POS 7


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
	int startCnt = -1;//最初からAssertの可能性もあるので-1にしとく
	int endCnt = -1;

	GPIO_STATE_t prevState = GetPinState();

	//タイマーをリセット
	ClearTimerCount();

	while(GetTimerCount() < T_RSTH){
		//ピンの状態を取得
		GPIO_STATE_t currentStatus = GetPinState();

		//立ち下がりを待つ
		if(prevState == GPIO_NEGATED && currentStatus == GPIO_ASSERTED){
			startCnt = GetTimerCount();
		}

		//立ち上がりを待つ
		if(startCnt > 0){//立ち下がり検出後
			if(prevState == GPIO_ASSERTED && currentStatus == GPIO_NEGATED){
				endCnt = GetTimerCount();
			}
		}

		prevState = currentStatus;
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
void SearchRom()
{

}
ONE_WIRE_ROM_CODE_t ReadRom()
{
	ONE_WIRE_ROM_CODE_t result;

	//ReadROM命令
	WriteByte(CODE_READ_ROM);

	//64bitコードで帰ってくる.
	unsigned char data[ROM_CODE_BYTE_SIZE] = {0};
	int cnt = 0;
	for(cnt = 0; cnt < ROM_CODE_BYTE_SIZE; cnt++){
		data[cnt] = ReadByte();
	}

	result.Family_Code = data[FAMILY_CODE_POS];
	result.SerialNumber_L = (unsigned short)data[SERIAL_NUMBER_LL_POS] | ((unsigned short)data[SERIAL_NUMBER_LH_POS] << BYTE_SIZE);
	result.SerialNumber_L = (unsigned short)data[SERIAL_NUMBER_ML_POS] | ((unsigned short)data[SERIAL_NUMBER_MH_POS] << BYTE_SIZE);
	result.SerialNumber_L = (unsigned short)data[SERIAL_NUMBER_HL_POS] | ((unsigned short)data[SERIAL_NUMBER_HH_POS] << BYTE_SIZE);
	result.CRC_Code = data[CRC_CODE_POS];

	return result;
}
void MatchRom()
{

}
void SkipRom()
{

}
