###Makefile
###-lstdc++#lib std c++
CC=g++
STD=_GNU_SOURCE
SRCS=launcher.cc
OBJS=launcher.o
OUTF=runnable.run
all: clean build
build:
	$(CC) $(SRCS) -o $(OUTF) -std=c++11
run:
	./$(OUTF) 25566
clean:
	rm -f *.o
clober: clean 
	rm -f $(OUTF)
kill:
	pkill $(OUTF)
###End Makefile
