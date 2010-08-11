#ifndef __BLOCKRANGEDECODER_H__
#define __BLOCKRANGEDECODER_H__

#define BUFFER_SIZE 4096

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct RangeDecoder
{
	uint32_t range,code;
	uint8_t buffer[BUFFER_SIZE];
	uint32_t bufferPos;
	void (*bufferFillCallback)(uint8_t*, uint32_t);
} RangeDecoder;

void InitRangeDecoder(RangeDecoder *self, void (*bufferFillCallback)(uint8_t*, uint32_t));
int ReadBitAndUpdateWeight(RangeDecoder *self,uint16_t *weight,int shift);
uint32_t ReadBitString(RangeDecoder *self,int length,uint16_t *weights,int shift);
uint32_t ReadBitStringWithVariableWeights(RangeDecoder *self,int length,uint16_t *weights,int *shifts);
uint32_t ReadUniversalCode(RangeDecoder *self,uint16_t *weights1,int shift1,uint16_t *weights2,int shift2);

#endif
