#include "AudioModel.h"
#include "RangeDecoder.h"

#include <stdio.h>

int main(int argc,char **argv)
{
	if(argc!=3)
	{
		fprintf(stderr,"Usage: %s file1.10bit.raw file2.10bit.raw\n",argv[0]);
		return 1;
	}

	FILE *fh1=fopen(argv[1],"rb");
	if(!fh1)
	{
		fprintf(stderr,"Can't open file \"%s\".\n",argv[1]);
		return 1;
	}

	FILE *fh2=fopen(argv[2],"rb");
	if(!fh2)
	{
		fprintf(stderr,"Can't open file \"%s\".\n",argv[2]);
		return 1;
	}


	for(int i=0;;i++)
	{
		uint8_t bytes1[2],bytes2[2];
		if(fread(bytes1,1,2,fh1)!=2) break;
		if(fread(bytes2,1,2,fh2)!=2) break;

		if(bytes1[1]!=bytes2[1]||
		(bytes1[0]&0xc0)!=(bytes2[0]&0xc0))
		{
			fprintf(stderr,"Difference at sample %d!\n",i);
			return 2;
		}
	}
}
