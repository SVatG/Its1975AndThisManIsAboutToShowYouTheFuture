CFLAGS=-std=c99 -g -O3

all: Encoder

Encoder: Encoder.o RangeEncoder.o AudioModel.o
	gcc -o $@ $^

AudioModel.o: AudioModel.c AudioModel.h
Encoder.o: Encoder.c AudioModel.h RangeEncoder.h
RangeDecoder.o: RangeDecoder.c RangeDecoder.h
RangeEncoder.o: RangeEncoder.c RangeEncoder.h
