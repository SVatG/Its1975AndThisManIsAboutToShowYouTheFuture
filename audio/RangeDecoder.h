#ifndef __RANGEDECODER_H__
#define __RANGEDECODER_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct RangeDecoder
{
	uint32_t range,code;

	int fd;
} RangeEncoder;

void InitRangeDecoder(RangeDecoder *self,int fd);
int ReadBitAndUpdateWeight(RangeDecoder *self,uint16_t *weight,int shift);
uint32_t ReadUniversalCode(RangeDecoder *self,uint16_t *weights1,int shift1,uint16_t *weights2,int shift2);

#endif
