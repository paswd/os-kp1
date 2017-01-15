FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -pthread
COMPILLER=g++

all: start

start: main.o
	$(COMPILLER) $(FLAGS) -o sea-battle main.o

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp

clear:
	-rm -f *.o *.gch sea-battle
