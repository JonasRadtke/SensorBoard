
#ifndef STRUCTS_H_
#define STRUCTS_H_

typedef volatile struct
{
	uint32_t gasImp;
	uint32_t stromImp;
	uint32_t strom;
	uint32_t leistung;
	
} current;

typedef volatile struct
{
	uint16_t memoryadress;
	uint16_t memorylength;
	uint8_t rx0Buffer[600];
	uint8_t tx0Buffer[600];
	uint32_t rx0Length;
	uint32_t tx0Length;
	uint32_t rx0Active;
	uint32_t rx0Done;
	uint32_t timeDone;
} comm;

#endif /* STRUCTS_H_ */