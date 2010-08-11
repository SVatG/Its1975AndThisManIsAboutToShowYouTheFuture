#ifndef __BLOCKDECODER_H__
#define __BLOCKDECODER_H__

void initDecoder(void (*bufferFillCallback)(uint8_t*, uint32_t));
void getBlock(uint16_t* samples, int size);

#endif
