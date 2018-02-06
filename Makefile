######################################
# Makefile for CS211 assignment 2
# Author: Rob Henderson
######################################

COMPILER = gcc
CCFLAGS  = -ansi -pedantic -Wall -lm
all: format

debug:
	make DEBUG=TRUE

format: format.o 
	$(COMPILER) $(CCFLAGS) -o format format.o
format.o: format.c format.h
	$(COMPILER) $(CCFLAGS) -c format.c

ifeq ($(DEBUG), TRUE)
CC += -g 
endif

clean:
	rm -f format
	rm -f *.o

