#include "AudioModel.h"

#include <stdint.h>
#include <string.h>

static inline int iabs(int x) { return x<0?-x:x; }

void InitAudioModel(AudioModel *self)
{
	memset(self,0,sizeof(*self));
}

inline int PredictNextSampleFromModel(AudioModel *self)
{
	return (
		8*self->lastsample+
		self->weight1*self->delta1+
		self->weight2*self->delta2+
		self->weight3*self->delta3
	)>>3;
}

inline void UpdateModelForRealSampleValue(AudioModel *self,int predicted,int sample)
{
//	int sample=predicted-delta&BitMask;

	int prederror=predicted-sample<<3;
	self->error[0]+=iabs(prederror);
	self->error[1]+=iabs(prederror-self->delta1);
	self->error[2]+=iabs(prederror+self->delta1);
	self->error[3]+=iabs(prederror-self->delta2);
	self->error[4]+=iabs(prederror+self->delta2);
	self->error[5]+=iabs(prederror-self->delta3);
	self->error[6]+=iabs(prederror+self->delta3);

	int lastdelta=sample-self->lastsample;
	self->delta3=self->delta2;
	self->delta2=lastdelta-self->delta1;
	self->delta1=lastdelta;

	self->lastsample=sample;

	if((self->count&0x1f)==0)
	{
		int minerror=self->error[0];
		int minindex=0;
		for(int i=1;i<7;i++)
		{
			if(self->error[i]<minerror)
			{
				minerror=self->error[i];
				minindex=i;
			}
		}

		memset(self->error,0,sizeof(self->error));

		switch(minindex)
		{
			case 1: if(self->weight1>=-16) self->weight1--; break;
			case 2: if(self->weight1<16) self->weight1++; break;
			case 3: if(self->weight2>=-16) self->weight2--; break;
			case 4: if(self->weight2<16) self->weight2++; break;
			case 5: if(self->weight3>=-16) self->weight3--; break;
			case 6: if(self->weight3<16) self->weight3++; break;
		}
	}

	self->count++;
}
