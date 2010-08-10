CFLAGS=-std=c99 -g -O3

all: Encoder Decoder 10BitCompare

Encoder: Encoder.o RangeEncoder.o AudioModel.o
	gcc -o $@ $^

Decoder: Decoder.o RangeDecoder.o AudioModel.o
	gcc -o $@ $^

10BitCompare: 10BitCompare.o
	gcc -o $@ $^

10BitCompare.o: 10BitCompare.c AudioModel.h RangeDecoder.h
AudioModel.o: AudioModel.c AudioModel.h
Decoder.o: Decoder.c AudioModel.h RangeDecoder.h
Encoder.o: Encoder.c AudioModel.h RangeEncoder.h
RangeDecoder.o: RangeDecoder.c RangeDecoder.h
RangeEncoder.o: RangeEncoder.c RangeEncoder.h
