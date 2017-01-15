FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -pthread
COMPILLER=g++

all: start

start: geometry.o battleobjects.o main.o
	$(COMPILLER) $(FLAGS) -o sea-battle geometry.o battleobjects.o main.o

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp

geometry.o: geometry.cpp
	$(COMPILLER) -c $(FLAGS) geometry.cpp

battleobjects.o: battleobjects.cpp
	$(COMPILLER) -c $(FLAGS) battleobjects.cpp

clear:
	-rm -f *.o *.gch sea-battle
