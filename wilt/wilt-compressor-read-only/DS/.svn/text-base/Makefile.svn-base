OBJS=WiltDS.o Test.o

CFLAGS=-std=c99

WiltDSTest: $(OBJS)
	$(CC) -o $@ $(OBJS) -lm

Test.o: Test.c WiltDS.h
WiltDS.o: WiltDS.c WiltDS.h
