CXXFLAGS= -Wall -Wextra -O0
src_folder = include
.PHNOY: all

all : install

install: dynamic_buffer.o runtime.o eratostene.o
	gcc $(CXXFLAGS) -g dynamic_buffer.o runtime.o eratostene.o -o eratostene

dynamic_buffer.o : $(src_folder)/dynamic_buffer.c
	gcc $(CXXFLAGS) -c -g $(src_folder)/dynamic_buffer.c

runtime.o : $(src_folder)/runtime.c
	gcc $(CXXFLAGS) -c -g $(src_folder)/runtime.c

eratostene.o : $(src_folder)/eratostene.c
	gcc $(CXXFLAGS) -c -g $(src_folder)/eratostene.c


clean : 
	rm eratostene.o dynamic_buffer.o runtime.o eratostene
