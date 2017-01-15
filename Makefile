FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -pthread
COMPILLER=g++

all: start

start: TextProcessor.o main.o
	$(COMPILLER) $(FLAGS) -o sea-battle TextProcessor.o main.o

TextProcessor.o: TextProcessor.cpp
	$(COMPILLER) -c $(FLAGS) TextProcessor.cpp

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp

clear:
	-rm -f *.o *.gch sea-battle
