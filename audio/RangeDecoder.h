#ifndef __RANGEDECODER_H__
#define __RANGEDECODER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct RangeDecoder
{
	uint32_t range,code;

	FILE *fh;
} RangeDecoder;

void InitRangeDecoder(RangeDecoder *self,FILE *fh);
int ReadBitAndUpdateWeight(RangeDecoder *self,uint16_t *weight,int shift);
uint32_t ReadBitString(RangeDecoder *self,int length,uint16_t *weights,int shift);
uint32_t ReadBitStringWithVariableWeights(RangeDecoder *self,int length,uint16_t *weights,int *shifts);
uint32_t ReadUniversalCode(RangeDecoder *self,uint16_t *weights1,int shift1,uint16_t *weights2,int shift2);

#endif
