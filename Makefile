CXXFLAGS= -Wall -Wextra -O0

.PHNOY: all

all : install

install: dynamic_buffer.o runtime.o eratostene.o
	gcc $(CXXFLAGS) -g dynamic_buffer.o runtime.o eratostene.o -o eratostene

dynamic_buffer.o : dynamic_buffer.c
	gcc $(CXXFLAGS) -c -g dynamic_buffer.c

runtime.o : runtime.c
	gcc $(CXXFLAGS) -c -g runtime.c

eratostene.o : eratostene.c
	gcc $(CXXFLAGS) -c -g eratostene.c


clean : 
	rm eratostene.o dynamic_buffer.o runtime.o eratostene
