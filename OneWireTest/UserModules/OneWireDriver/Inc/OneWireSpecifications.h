/*
 * OneWireSpecifications.h
 *
 *  Created on: 2018/07/13
 *      Author: Shuji
 */

#ifndef ONEWIREDRIVER_INC_ONEWIRESPECIFICATIONS_H_
#define ONEWIREDRIVER_INC_ONEWIRESPECIFICATIONS_H_

//Time slot in usec
#define T_SLOT     (uint16_t)90
#define T_SLOT_MAX (uint16_t)120
#define T_SLOT_MIN (uint16_t)60

//Recovery time in usec
#define T_REC (uint16_t)1

//Write 0 Low time in usec
#define T_LOW0     (uint16_t)90
#define T_LOW0_MAX (uint16_t)120
#define T_LOW0_MIN (uint16_t)60

//Write 1 Low time in usec
#define T_LOW1     (uint16_t)8
#define T_LOW1_MAX (uint16_t)15
#define T_LOW1_MIN (uint16_t)1

//Read Data Valid in usec
#define T_RDV (uint16_t)15

//Reset time high in usec
#define T_RSTH (uint16_t)480

//Reset time low in usec
#define T_RSTL (uint16_t)480

//Presense detect high in usec
#define T_PDHIGH (uint16_t)40
#define T_PDHIGH_MAX (uint16_t)60
#define T_PDHIGH_MIN (uint16_t)15

//Presense detect low in usec
#define T_PDLOW (uint16_t)150
#define T_PDLOW_MAX (uint16_t)240
#define T_PDLOW_MIN (uint16_t)60

#endif /* ONEWIREDRIVER_INC_ONEWIRESPECIFICATIONS_H_ */
