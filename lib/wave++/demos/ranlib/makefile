#makefile to create archive of ranlib code

CC= g++
CFLAGS=	-O2  -Wall 
CLIBS= -lm -liostream

all: ranlib

ranlib:
	$(CC) $(CFLAGS) -c com.c
	$(CC) $(CFLAGS) -c linpack.c
	$(CC) $(CFLAGS) -c ranlib.c
	ar rcs libranlib.a	\
		com.o	        \
		linpack.o	\
		ranlib.o	        
			        
clean:
	rm -f *.o *~ 

# End