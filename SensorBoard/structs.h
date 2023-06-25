/*
 * structs.h
 *
 * Created: 25.01.2022 18:43:16
 *  Author: Radtke
 */ 


#ifndef STRUCTS_H_
#define STRUCTS_H_


#define MODBUSBAUDRATE						19200
#define SLAVEID								1				// 1 - 247
#define COILS_START_ADRESS					1				// 1 - 9999
#define MAXCOILS							8				// 1 - 9999, discrete Outputs rw from Master
#define INPUTS_START_ADRESS					10001			// 10001 - 19999
#define MAXINPUTS							8				// 10001 - 19999, discrete inputs r from Master
#define INPUTREGISTERS_START_ADRESS			30001			// 30001 - 39999
#define MAXINPUTREGISTERS					20				// 30001 - 39999, analog inputs r from Master
#define OUTPUTREGISTERS_START_ADRESS		40001			// 40001 - 49999
#define MAXOUTPUTREGISTERS					20				// 40001 - 49999, analog outputs rw from Master


typedef volatile struct
{
	uint32_t gasImp;
	uint32_t stromImp;
	
} current;

typedef volatile struct
{
	uint8_t outputcoils[(MAXCOILS/8)+1];
	uint8_t inputcoils[(MAXINPUTS/8)+1];
	uint16_t inputregisters[MAXINPUTREGISTERS+1];
	uint16_t outputregisters[MAXOUTPUTREGISTERS+1];
	uint16_t memoryadress;
	uint16_t memorylength;
	uint8_t rxBuffer[300];
	uint8_t txBuffer[300];
	uint32_t rxLength;
	uint32_t txLength;
	uint32_t rxActive;
	uint32_t txActive;
	uint32_t rxDone;
	uint32_t timeDone;
} comm;

#endif /* STRUCTS_H_ */