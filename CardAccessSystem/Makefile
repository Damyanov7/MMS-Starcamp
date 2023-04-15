CC=gcc -g
COMPILE=$(CC) -c
LINK=$(CC)

.c.o:
	$(COMPILE) -I. $< -o $@
	
Card_Access_System: ccs.o sm.o 
	$(LINK)  ccs.o sm.o -o $@
	
all: Card_Access_System

clean: 
	rm -f Card_Access_System *.o
