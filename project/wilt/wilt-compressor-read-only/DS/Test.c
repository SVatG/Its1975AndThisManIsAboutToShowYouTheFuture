#include "WiltDS.h"

#include <unistd.h>
#include <stdlib.h>

// A simple test case to run the code on a desktop machine.
// Only works on little-endian machines.

int main(int argc,char **argv)
{
	uint32_t size;
	read(0,&size,4);

	uint32_t shifts=0;
	read(0,&shifts,3);

	uint16_t *buf=malloc(size);
	DecompressData(0,buf,size,shifts>>20,(shifts>>16)&0xf,(shifts>>12)&0xf,(shifts>>8)&0xf,(shifts>>4)&0xf,shifts&0xf);
	write(1,buf,size);
}
