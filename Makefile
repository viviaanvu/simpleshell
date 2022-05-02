#
# Project 2
# Vivian Vu
#

# Make environment
CC=gcc
CFLAGS=-c -g -Wall 
LINKARGS=-lm


# Files
OBJECT_FILES=	p2.o p2-support.o

# Productions
all : cmsc257-f20-p2

cmsc257-f20-p2 : $(OBJECT_FILES)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@

p2.o : cmsc257-f20-p2.c p2-support.h
	$(CC) $(CFLAGS) $< -o $@

p2-support.o : p2-support.c p2-support.h 
	$(CC) $(CFLAGS) $< -o $@

clean : 
	rm -v cmsc257-f20-p2 $(OBJECT_FILES)
