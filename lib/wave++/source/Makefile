# Makefile for the whole library 

CC = g++
CFLAGS = -Wall

all: wlib

wlib:
	$(CC) -O2 $(CFLAGS) -c common.cc
	$(CC) -O2 $(CFLAGS) -c Interval.cc
	$(CC) -O2 $(CFLAGS) -c ArrayTree.cc
	$(CC) -O2 $(CFLAGS) -c Hedge.cc
	$(CC) -O2 $(CFLAGS) -c Utility.cc
	$(CC) -O2 $(CFLAGS) -c InOutUtil.cc
	$(CC) -O2 $(CFLAGS) -c WavePacket.cc
	$(CC) -O2 $(CFLAGS) -c WaveTrans.cc
	$(CC) -O2 $(CFLAGS) -c ConvDec.cc
	$(CC) -O2 $(CFLAGS) -c QMF.cc
	$(CC) -O2 $(CFLAGS) -c fft.c
	$(CC) -O2 $(CFLAGS) -c Gabor.cc
	$(CC) -O2 $(CFLAGS) -c Partition.cc
	$(CC) -O2 $(CFLAGS) -c ShiftGaborMP.cc
	$(CC) -O2 $(CFLAGS) -c FFTGaborMP.cc
	ar rcvs libw.a *.o
	ranlib libw.a

common.o: common.h common.cc
	$(CC) -c $(CFLAGS) -o common.o common.cc

Interval.o: Interval.h Interval.cc
	$(CC) -c $(CFLAGS) -o Interval.o Interval.cc

ArrayTree.o: Interval.h ArrayTree.h ArrayTree.cc
	$(CC) -c $(CFLAGS) -o ArrayTree.o ArrayTree.cc

Hedge.o: Interval.h Hedge.h Hedge.cc
	$(CC) -c $(CFLAGS) -o Hedge.o Hedge.cc

InOutUtil.o: Interval.h Hedge.h ArrayTree.h InOutUtil.h InOutUtil.cc
	$(CC) -c $(CFLAGS) -o InOutUtil.o InOutUtil.cc

Utility.o: Interval.h Hedge.h ArrayTree.h BinTree.h Utility.h Utility.cc
	$(CC) -c $(CFLAGS) -o Utility.o Utility.cc

QMF.o: common.h QMF.h QMF.cc
	$(CC) -c $(CFLAGS) -o QMF.o QMF.cc

ConvDec.o: Interval.h QMF.h ConvDec.h ConvDec.cc
	$(CC) -c $(CFLAGS) -o Convdec.o ConvDec.cc

WavePacket.o: Interval.h ArrayTree.h ConvDec.h QMF.h WavePacket.h WavePacket.cc
	$(CC) -c $(CFLAGS) -o WavePacket.o WavePacket.cc

WaveTrans.o: Interval.h ConvDec.h QMF.h WaveTrans.h WaveTrans.cc
	$(CC) -c $(CFLAGS) -o WaveTrans.o WaveTrans.cc

fft.o: common.h  fft.h fft.c
	$(CC) -c $(CFLAGS) -o fft.o fft.c

Gabor.o: Gabor.h Interval.h Gabor.cc
	g++ $(CFLAGS) -c Gabor.cc

Partition.o: Interval.h Partition.h Partition.cc
	g++ $(CFLAGS) -c Partition.cc

ShiftGaborMP.o: ShiftGabor.h Partition.h  Gabor.h ShiftGaborMP.cc
	g++ $(CFLAGS) -c ShiftGaborMP.cc

FFTGaborMP.o: FFTGaborMP.h complex.h fft.h Gabor.h FFTGaborMP.cc
	g++ $(CFLAGS) -c FFTGaborMP.cc

clean: 
	rm  *.o *~ *.a