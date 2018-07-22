/*
 * DS18B20Driver.h
 *
 *  Created on: 2018/07/22
 *      Author: Shuji
 */

#ifndef DS18B20DRIVER_INC_DS18B20DRIVER_H_
#define DS18B20DRIVER_INC_DS18B20DRIVER_H_

void Convert();
void WriteScratchPad();
void ReadScratchPad();
void RecallE2();
void ReadPowerSupply();

#endif /* DS18B20DRIVER_INC_DS18B20DRIVER_H_ */
