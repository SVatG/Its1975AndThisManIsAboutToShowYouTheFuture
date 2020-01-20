#include "WiltDS.h"

#include <unistd.h>



typedef struct RangeDecoder
{
	uint32_t range,code;
	int fd;
} RangeDecoder;

static void InitRangeDecoder(RangeDecoder *self,int fd)
{
	self->fd=fd;
	self->range=0xffffffff;
	self->code=0;

	static uint8_t buf[4];
	read(fd,buf,4);
	for(int i=0;i<4;i++) self->code=(self->code<<8)|buf[i];
}

static void NormalizeRangeDecoder(RangeDecoder *self)
{
	if(self->range<0x1000000)
	{
		uint8_t val;
		read(self->fd,&val,1);
		self->code=(self->code<<8) | val;
		self->range<<=8;
	}
}

static int ReadBitAndUpdateWeight(RangeDecoder *self,uint16_t *weight,int shift)
{
	NormalizeRangeDecoder(self);

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

static uint32_t ReadUniversalCode(RangeDecoder *self,uint16_t *weights1,int shift1,uint16_t *weights2,int shift2)
{
	int numbits=0;

	while(ReadBitAndUpdateWeight(self,&weights1[numbits],shift1)==1) numbits++;
	if(!numbits) return 0;

	uint32_t val=1;
	for(int i=0;i<numbits-1;i++)
	val=(val<<1)|ReadBitAndUpdateWeight(self,&weights2[numbits-1-i],shift2);

	return val;
}




static void CopyMemory(uint16_t *dest,uint16_t *src,int length)
{
	for(int i=0;i<length;i++) *dest++=*src++;
}

void DecompressData(int fd,uint16_t *buf,uint32_t size,
int typeshift,int literalshift,int lengthshift1,int lengthshift2,int offsetshift1,int offsetshift2)
{
	RangeDecoder dec;
	InitRangeDecoder(&dec,fd);

	uint16_t typeweight=0x800;

	uint16_t lengthweights1[32],lengthweights2[32];
	uint16_t offsetweights1[32],offsetweights2[32];
	for(int i=0;i<32;i++)
	lengthweights1[i]=lengthweights2[i]=offsetweights1[i]=offsetweights2[i]=0x800;

	uint16_t literalbitweights[16][16];
	for(int i=0;i<16;i++)
	for(int j=0;j<16;j++)
	literalbitweights[i][j]=0x800;

	int pos=0;
	while(pos<size/2)
	{
		int length,offs;

		if(ReadBitAndUpdateWeight(&dec,&typeweight,typeshift)==1)
		{
			int length=ReadUniversalCode(&dec,lengthweights1,lengthshift1,lengthweights2,lengthshift2)+2;
			int offs=ReadUniversalCode(&dec,offsetweights1,offsetshift1,offsetweights2,offsetshift2)+1;

			CopyMemory(&buf[pos],&buf[pos-offs],length);

			pos+=length;
		}
		else
		{
			uint16_t val=0;

			for(int i=0;i<16;i++)
			{
				int bit=ReadBitAndUpdateWeight(&dec,&literalbitweights[i][val&15],literalshift);
				val=(val<<1)|bit;
			}
			buf[pos]=val;

			pos++;
		}
	}
}
