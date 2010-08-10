#ifndef __AUDIOMODEL_H__
#define __AUDIOMODEL_H__

typedef struct AudioModel
{
	int weight1,weight2,weight3,weight4,weight5;
	int delta1,delta2,delta3,delta4;
	int error[7];
	int count;
	int lastsample;
} AudioModel;

void InitAudioModel(AudioModel *self);
int PredictNextSampleFromModel(AudioModel *self);
void UpdateModelForRealSampleValue(AudioModel *self,int predicted,int sample);

#endif
