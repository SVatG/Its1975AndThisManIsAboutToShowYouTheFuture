#include "RangeDecoder.h"

#include <unistd.h>



static inline uint8_t GetByte(RangeDecoder *self)
{
	uint8_t byte;
	read(self->fd,&byte,1);
	return byte;
}

void InitRangeDecoder(RangeDecoder *self,int fd)
{
	self->range=0xffffffff;
	self->code=0;
	self->fd=fd;

	for(int i=0;i<4;i++) self->code=(self->code<<8)|GetByte(self);
}

int ReadBitAndUpdateWeight(RangeDecoder *self,uint16_t *weight,int shift)
{
	// Normalize if needed.
	if(self->range<0x1000000)
	{
		self->code=(self->code<<8)|GetByte(self);
		self->range<<=8;
	}

	uint32_t threshold=(self->range>>12)*(*weight);

	if(self->code<threshold)
	{
		self->range=threshold;
		*weight+=(0x1000-*weight)>>shift;
		return 0;
	}
	else
	{
		self->range-=threshold;
		self->code-=threshold;
		*weight-=*weight>>shift;
		return 1;
	}
}

uint32_t ReadUniversalCode(RangeDecoder *self,uint16_t *weights1,int shift1,uint16_t *weights2,int shift2)
{
	int numbits=0;

	while(ReadBitAndUpdateWeight(self,&weights1[numbits],shift1)==1) numbits++;
	if(!numbits) return 0;

	uint32_t val=1;

	for(int i=0;i<numbits-1;i++)
	val=(val<<1)|ReadBitAndUpdateWeight(self,&weights2[numbits-1-i],shift2);

	return val;
}

