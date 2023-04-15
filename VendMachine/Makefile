CC=gcc
COMPILE=$(CC)
LINK=$(CC)

.c.o:
	$(COMPILE) -c -I. $< -o $@

number: sm.h sm.o vend_machine.o
	$(LINK) sm.o vend_machine.o -o $@

all: number

clean:
	rm -f number *.o

