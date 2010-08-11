#ifndef __AUDIOMODEL_H__
#define __AUDIOMODEL_H__

#define MAX_ORDER 3

typedef struct AudioModel
{
	int weight[MAX_ORDER+1];
	int delta[MAX_ORDER+1];
	int error[MAX_ORDER*2+3];
	int count;
	int lastsample;
} AudioModel;

void InitAudioModel(AudioModel *self);
int PredictNextSampleFromModel(AudioModel *self);
void UpdateModelForRealSampleValue(AudioModel *self,int predicted,int sample);

#endif
