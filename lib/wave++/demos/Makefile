#Makefile for directory demos
CFLAGS =# -g #debugging disabled 

OBJDEMOWAV =  demoWav.o input_maker.o rms_error.o demoTools.o
OBJDEMOWAVPACK =  demoWavPack.o input_maker.o rms_error.o demoTools.o
OBJDEMOMP =  demoMP.o input_maker.o 
LINKS =    -L./ranlib  -L../source -lranlib -lm  -lw


demo: demoWav demoWavPack demoMP


demoWav: $(OBJDEMOWAV)
	g++  $(CFLAGS) -o demoWav $(OBJDEMOWAV) $(LINKS)

demoWav.o: demoWav.cc
	g++ $(CFLAGS) -c demoWav.cc

demoWavPack: $(OBJDEMOWAVPACK)
	g++  $(CFLAGS) -o demoWavPack $(OBJDEMOWAVPACK) $(LINKS)

demoWavPack.o: demoWavPack.cc
	g++ $(CFLAGS) -c demoWavPack.cc


demoTools.o: demoTools.cc demoTools.h
	g++ $(CFLAGS) -c demoTools.cc


demoMP: $(OBJDEMOMP)
	g++  $(CFLAGS) -o demoMP $(OBJDEMOMP) $(LINKS)


demoMP.o: demoMP.cc
	g++ $(CFLAGS) -c demoMP.cc

input_maker.o: ./input_maker.cc ./input_maker.h
	g++ $(CFLAGS) -c input_maker.cc 

rms_error.o: ./rms_error.h ./rms_error.cc 
	g++ $(CFLAGS) -c rms_error.cc 

clean: 
	-rm demoWav demoWavPack demoMP *.o *~ 










