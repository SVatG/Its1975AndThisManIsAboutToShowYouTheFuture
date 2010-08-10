CFLAGS=-std=c99 -g -O3

all: Encoder Decoder

Encoder: Encoder.o RangeEncoder.o AudioModel.o
	gcc -o $@ $^

Decoder: Decoder.o RangeDecoder.o AudioModel.o
	gcc -o $@ $^

AudioModel.o: AudioModel.c AudioModel.h
Encoder.o: Encoder.c AudioModel.h RangeEncoder.h
RangeDecoder.o: RangeDecoder.c RangeDecoder.h
RangeEncoder.o: RangeEncoder.c RangeEncoder.h
