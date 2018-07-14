/*
 * GPIO_Wrapper.h
 *
 *  Created on: 2018/07/13
 *      Author: Shuji
 */

#ifndef ONEWIREDRIVER_INC_GPIO_WRAPPER_H_
#define ONEWIREDRIVER_INC_GPIO_WRAPPER_H_

typedef enum{
	GPIO_ASSERTED,
	GPIO_NEGATED
}GPIO_STATE_t;

void AssertPin();
void NegatePin();
void SetPin2TxMode();
void SetPin2RxMode();
GPIO_STATE_t GetPinState();

#endif /* ONEWIREDRIVER_INC_GPIO_WRAPPER_H_ */
