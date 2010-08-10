#include "AudioModel.h"
#include "RangeDecoder.h"

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
	RangeDecoder coder;
	InitRangeDecoder(&coder,stdin);

	AudioModel model;
	InitAudioModel(&model);

	uint16_t context[NumContexts];
	for(int i=0;i<NumContexts;i++) context[i]=0x800;

	for(;;)
	{
		int predicted=PredictNextSampleFromModel(&model);

		int delta=ReadBitStringWithVariableWeights(&coder,
		NumBits,context,(int[]){7,4,5,5,5,5,6,6,6,7});

		int sample=SignExtend(predicted-delta&BitMask,NumBits);

		UpdateModelForRealSampleValue(&model,predicted,sample);

		sample<<=16-NumBits;

		fputc(sample&0xff,stdout);
		fputc((sample>>8)&0xff,stdout);

		if(feof(stdin)) break;
	}
}
