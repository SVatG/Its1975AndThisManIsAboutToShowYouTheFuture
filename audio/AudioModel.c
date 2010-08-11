#include "AudioModel.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

static inline int iabs(int x) { return x<0?-x:x; }

void InitAudioModel(AudioModel *self)
{
	memset(self,0,sizeof(*self));
}

int PredictNextSampleFromModel(AudioModel *self)
{
	int prediction = 8*self->lastsample;
	for( int i = 0; i <= MAX_ORDER; i++ ) {
		prediction += self->weight[i]*self->delta[i];
	}
	return( prediction>>3 );
}

void UpdateModelForRealSampleValue(AudioModel *self,int predicted,int sample)
{
//	int sample=predicted-delta&BitMask;

	int prederror=predicted-sample<<3;
	self->error[0]+=iabs(prederror);
	for( int i = 0; i <= MAX_ORDER; i++ ) {
		self->error[2*i+1]+=iabs(prederror-self->delta[i]);
		self->error[2*i+2]+=iabs(prederror+self->delta[i]);
	}
	
	if((self->count&0x1f)==0)
	{
		int minindex=0;
		int minerror=self->error[0];
		for(int i=1;i<=(MAX_ORDER*2+2);i++)
		{
			if(self->error[i]<minerror)
			{
				minerror=self->error[i];
				minindex=i;
			}
		}

		if( minindex != 0 ) {
			if( minindex%2 != 0 ) {
				if( self->weight[(minindex-1)/2] >= -32 ) {
					self->weight[(minindex-1)/2]--;
				}
			}
			else {
				if( self->weight[(minindex-1)/2] < 32 ) {
					self->weight[(minindex-1)/2]++;
				}
			}
		}
		memset(self->error,0,sizeof(self->error));
	}

	int lastdelta=sample-self->lastsample;
	for( int i = MAX_ORDER; i > 1; i-- ) {
		self->delta[i]=self->delta[i-2]-self->delta[i-1];
	}
	self->delta[1]=lastdelta-self->delta[0];
	self->delta[0]=lastdelta;

	self->lastsample=sample;

	self->count++;
}
