#include "AudioModel.h"
#include "RangeEncoder.h"

#include <stdio.h>

#define NumBits 10
#define NumContexts (1<<(NumBits+1))
#define BitMask ((1<<NumBits)-1)

static int SignExtend(int val,int bits)
{
	return val|~((val&(1<<bits-1))-1);
}

int main(int argc,char **argv)
{
	RangeEncoder coder;
	InitRangeEncoder(&coder,stdout);

	AudioModel model;
	InitAudioModel(&model);

	uint16_t context[NumContexts];
	for(int i=0;i<NumContexts;i++) context[i]=0x800;

	for(;;)
	{
		uint8_t bytes[2];
		if(fread(bytes,1,2,stdin)!=2) break;

		int sample=SignExtend(bytes[0]|(bytes[1]<<8),16);
		sample>>=16-NumBits;

		int predicted=PredictNextSampleFromModel(&model);
		UpdateModelForRealSampleValue(&model,predicted,sample);

//		printf("%02x%02x %d %d %d\n",bytes[1],bytes[0],sample,predicted,sample-predicted);
		WriteBitString(&coder,sample-predicted,NumBits,context,4);
	}

//	FinishRangeEncoder(&coder);
}
