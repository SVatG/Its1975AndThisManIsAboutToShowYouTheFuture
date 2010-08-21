#include "AudioModel.h"
#include "BlockRangeDecoder.h"
#include "BlockDecoder.h"

#define NumBits 10
#define NumContexts (1<<(NumBits+1))
#define BitMask ((1<<NumBits)-1)

static inline int SignExtend(int val,int bits)
{
	return val|~((val&(1<<bits-1))-1);
}

static RangeDecoder coder;
AudioModel model;
uint16_t context[NumContexts];

void initDecoder(void (*bufferFillCallback)(uint8_t*, uint32_t)) {
	InitRangeDecoder(&coder,bufferFillCallback);
	InitAudioModel(&model);
	for(int i=0;i<NumContexts;i++) context[i]=0x800;
}

void getBlock(int16_t* samples, int size) {
	for(int i = 0; i < size; i++) {
		int predicted=PredictNextSampleFromModel(&model);
		int delta=ReadBitStringWithVariableWeights(&coder,
		NumBits,context,(int[]){7,4,5,5,5,5,6,6,6,7});
		samples[i]=SignExtend(predicted-delta&BitMask,NumBits);
		UpdateModelForRealSampleValue(&model,predicted,samples[i]);
		samples[i]<<=16-NumBits;
	}
}
