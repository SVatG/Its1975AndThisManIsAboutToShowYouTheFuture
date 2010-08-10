#ifndef __RANGEENCODER_H__
#define __RANGEENCODER_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct RangeEncoder
{
	uint32_t range;
	uint64_t low;
	uint8_t cache;
	uint32_t cachesize;

	int fd;
} RangeEncoder;

void InitRangeEncoder(RangeEncoder *self,int fd);
void WriteBitAndUpdateWeight(RangeEncoder *self,int bit,uint16_t *weight,int shift);
void WriteBitString(RangeEncoder *self,uint32_t value,int length,uint16_t *weights,int shift);
void WriteUniversalCode(RangeEncoder *self,uint32_t value,uint16_t *weights1,int shift1,uint16_t *weights2,int shift2);
void FinishRangeEncoder(RangeEncoder *self);

double CalculateCostOfBit(int bit,uint16_t *weight,int shift,bool updateweight);
double CalculateCostOfUniversalCode(uint32_t value,uint16_t *weights1,int shift1,uint16_t *weights2,int shift2,bool updateweight);

#endif
