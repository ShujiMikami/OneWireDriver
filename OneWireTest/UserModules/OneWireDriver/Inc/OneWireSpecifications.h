/*
 * OneWireSpecifications.h
 *
 *  Created on: 2018/07/13
 *      Author: Shuji
 */

#ifndef ONEWIREDRIVER_INC_ONEWIRESPECIFICATIONS_H_
#define ONEWIREDRIVER_INC_ONEWIRESPECIFICATIONS_H_

//Time slot in usec
#define T_SLOT     (unsigned short)90
#define T_SLOT_MAX (unsigned short)120
#define T_SLOT_MIN (unsigned short)60

//Recovery time in usec
#define T_REC (unsigned short)1

//Write 0 Low time in usec
#define T_LOW0     (unsigned short)90
#define T_LOW0_MAX (unsigned short)120
#define T_LOW0_MIN (unsigned short)60

//Write 1 Low time in usec
#define T_LOW1     (unsigned short)8
#define T_LOW1_MAX (unsigned short)15
#define T_LOW1_MIN (unsigned short)1

//read time slot initiate
#define T_INT      (unsigned short)1

//Read Data Valid in usec
#define T_RDV (unsigned short)15

//Reset time high in usec
#define T_RSTH (unsigned short)480

//Reset time low in usec
#define T_RSTL (unsigned short)480

//Presense detect high in usec
#define T_PDHIGH (unsigned short)40
#define T_PDHIGH_MAX (unsigned short)60
#define T_PDHIGH_MIN (unsigned short)15

//Presense detect low in usec
#define T_PDLOW (unsigned short)150
#define T_PDLOW_MAX (unsigned short)240
#define T_PDLOW_MIN (unsigned short)60

#endif /* ONEWIREDRIVER_INC_ONEWIRESPECIFICATIONS_H_ */