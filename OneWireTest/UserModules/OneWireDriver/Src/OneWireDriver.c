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

#define MAX_DEVICE_COUNT (uint16_t)0xFF

#include "OneWireDriver.h"

#include "OneWireSpecifications.h"
#include "GPIO_Wrapper.h"
#include "Wait_Wrapper.h"

typedef enum{
	NOT_SEARCHED,
	BOTH_EXIST,
	ONLY_1_EXIST,
	ONLY_0_EXIST,
	NO_DEVICE
}SearchROMBitState_t;

static ONE_WIRE_ROM_CODE_t createROMStructFromBit(uint8_t bitData[64]);
static ONE_WIRE_ROM_CODE_t createROMStructFromByte(uint8_t byteData[8]);
static uint16_t numOfDeviceFound = 0;
static ONE_WIRE_ROM_CODE_t foundROMCode[MAX_DEVICE_COUNT];

static void write0();
static void write1();
static uint8_t readBit();
static void getByteArrayFromRomCode(ONE_WIRE_ROM_CODE_t data, uint8_t* buffer);
static ONE_WIRE_STATUS_t readPresensePulse();
static SearchROMBitState_t searchROMBitCheck();


ONE_WIRE_STATUS_t ResetPulse()
{
	//GPIOをTxモードに
	SetPin2TxMode();

	//GPIOをアサート
	AssertPin();

	//tRSTLだけ待つ
	Wait_us(T_RSTL);

	//GPIOをネゲート
	NegatePin();

	//PresensePulseを読む
	return readPresensePulse();
}
ONE_WIRE_STATUS_t readPresensePulse()
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
void WriteByte(uint8_t byteData)
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
uint8_t readBit()
{
	uint8_t result = 0;

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
uint8_t ReadByte()
{
	uint8_t result = 0;

	int cnt = 0;
	for(cnt = 0; cnt < BYTE_SIZE; cnt++){
		uint8_t currentBit = readBit();
		result |= (currentBit << cnt);
	}

	return result;
}
SearchROMBitState_t searchROMBitCheck()
{
	SearchROMBitState_t result = NO_DEVICE;

	//LSbのAND読み込み
	uint8_t andOfLsb = readBit();

	//LSbの反転のAND読み込み
	uint8_t andOfNotLsb = readBit();

	//両者を並べた数値作成
	uint8_t flag = (andOfLsb << 1) | andOfNotLsb;

	switch(flag){
	case 0://このビット位置は特定不能
		result = BOTH_EXIST;
		break;
	case 1://このビット位置は全デバイス0
		result = ONLY_0_EXIST;
		break;
	case 2://このビット位置は全デバイス1
		result = ONLY_1_EXIST;
		break;
	case 3://デバイスいない
		result = NO_DEVICE;
		break;
	}

	return result;
}
void SearchRom()
{
	//各ビットの状態
	SearchROMBitState_t bitState[64] = {NOT_SEARCHED};

	//各ビットの値
	uint8_t bitArray[64] = {0};



	while(1){
		ONE_WIRE_STATUS_t result = ResetPulse();

		if(result == ONE_WIRE_SUCCESS){
			//SearchROM命令
			WriteByte(CODE_SEARCH_ROM);

			//TODO while抜けを実装

			int cnt = 0;
			for(cnt = 0; cnt < 64; cnt++){
				//bit読み
				SearchROMBitState_t currentBitState = searchROMBitCheck();

				switch(currentBitState){
				case ONLY_0_EXIST: //0しか存在しない場合ビット確定して次
					write0();
					bitArray[cnt] = 0;
					break;
				case ONLY_1_EXIST: //1しか存在しない場合ビット確定して次
					write1();
					bitArray[cnt] = 1;
					break;
				case BOTH_EXIST:
					if(bitState[cnt] == NOT_SEARCHED){//0側をサーチしていない場合まず0をセット
						write0();
						bitArray[cnt] = 0;
					}
					else{//0側をサーチ済みの場合1をセット
						write1();
						bitArray[cnt] = 1;
					}
					break;
				case NO_DEVICE:
					break;
				case NOT_SEARCHED:
					break;
				}

				if(currentBitState == NO_DEVICE){//デバイス見つからずの場合はもうこのルートをサーチしない
					break;
				}
				else{//ビット値を格納して次へ
					bitState[cnt] = currentBitState;
				}
			}

			if(cnt >= 64){//最後のビットまで行ったらデータコピー
				foundROMCode[numOfDeviceFound] = createROMStructFromBit(bitArray);
				numOfDeviceFound++;

			}
		}
	}
}
ONE_WIRE_ROM_CODE_t ReadRom()
{
	ONE_WIRE_ROM_CODE_t result;

	//ReadROM命令
	WriteByte(CODE_READ_ROM);

	//64bitコードで帰ってくる.
	uint8_t data[ROM_CODE_BYTE_SIZE] = {0};
	int cnt = 0;
	for(cnt = 0; cnt < ROM_CODE_BYTE_SIZE; cnt++){
		data[cnt] = ReadByte();
	}

	result.Family_Code = data[FAMILY_CODE_POS];
	result.SerialNumber_L = (uint16_t)data[SERIAL_NUMBER_LL_POS] | ((uint16_t)data[SERIAL_NUMBER_LH_POS] << BYTE_SIZE);
	result.SerialNumber_L = (uint16_t)data[SERIAL_NUMBER_ML_POS] | ((uint16_t)data[SERIAL_NUMBER_MH_POS] << BYTE_SIZE);
	result.SerialNumber_L = (uint16_t)data[SERIAL_NUMBER_HL_POS] | ((uint16_t)data[SERIAL_NUMBER_HH_POS] << BYTE_SIZE);
	result.CRC_Code = data[CRC_CODE_POS];

	return result;
}
void MatchRom(ONE_WIRE_ROM_CODE_t romCodeToMatch)
{
	//MatchROM命令
	WriteByte(CODE_MATCH_ROM);

	//ROMコードを順次送信
	uint8_t data[ROM_CODE_BYTE_SIZE] = {0};
	getByteArrayFromRomCode(romCodeToMatch, data);

	int cnt = 0;
	for(cnt = 0; cnt < ROM_CODE_BYTE_SIZE; cnt++){
		WriteByte(data[cnt]);
	}
}
void SkipRom()
{
	//SkipROM命令
	WriteByte(CODE_SKIP_ROM);
}
void getByteArrayFromRomCode(ONE_WIRE_ROM_CODE_t data, uint8_t* buffer)
{
	buffer[FAMILY_CODE_POS] = data.Family_Code;
	buffer[SERIAL_NUMBER_LL_POS] = (uint8_t)data.SerialNumber_L;
	buffer[SERIAL_NUMBER_LH_POS] = (uint8_t)(data.SerialNumber_L >> BYTE_SIZE);
	buffer[SERIAL_NUMBER_ML_POS] = (uint8_t)data.SerialNumber_M;
	buffer[SERIAL_NUMBER_MH_POS] = (uint8_t)(data.SerialNumber_M >> BYTE_SIZE);
	buffer[SERIAL_NUMBER_HL_POS] = (uint8_t)data.SerialNumber_H;
	buffer[SERIAL_NUMBER_HH_POS] = (uint8_t)(data.SerialNumber_H >> BYTE_SIZE);
	buffer[CRC_CODE_POS] = data.CRC_Code;
}
uint8_t ReadSlot()
{
	return readBit();
}
ONE_WIRE_ROM_CODE_t createROMStructFromBit(uint8_t bitData[64])
{
	uint8_t byteData[8] = {0};
	int cnt = 0;
	for(cnt = 0; cnt < 8; cnt++){
		int i = 0;
		for(i = 0; i < 8; i++){
			byteData[cnt] |= (bitData[8 * cnt + i] << i);
		}
	}

	return createROMStructFromByte(byteData);
}
ONE_WIRE_ROM_CODE_t createROMStructFromByte(uint8_t byteData[8])
{
	ONE_WIRE_ROM_CODE_t result;

	//family Code
	result.Family_Code = byteData[FAMILY_CODE_POS];

	//Serial Number
	result.SerialNumber_L = (uint16_t)byteData[SERIAL_NUMBER_LL_POS] | ((uint16_t)byteData[SERIAL_NUMBER_LH_POS] << 8);
	result.SerialNumber_M = (uint16_t)byteData[SERIAL_NUMBER_ML_POS] | ((uint16_t)byteData[SERIAL_NUMBER_MH_POS] << 8);
	result.SerialNumber_H = (uint16_t)byteData[SERIAL_NUMBER_HL_POS] | ((uint16_t)byteData[SERIAL_NUMBER_HH_POS] << 8);

	//CRC Code
	result.CRC_Code = byteData[CRC_CODE_POS];

	return result;
}
