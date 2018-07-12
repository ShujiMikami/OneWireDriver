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
