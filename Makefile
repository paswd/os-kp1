FLAGS=-pedantic -Wall -Werror -Wno-sign-compare -Wno-long-long -lm -pthread
COMPILLER=g++

all: start

start: geometry.o converter.o battleobjects.o interface.o parser.o main.o
	$(COMPILLER) $(FLAGS) -o sea-battle geometry.o converter.o battleobjects.o interface.o parser.o main.o

main.o: main.cpp
	$(COMPILLER) -c $(FLAGS) main.cpp

geometry.o: geometry.cpp
	$(COMPILLER) -c $(FLAGS) geometry.cpp

battleobjects.o: battleobjects.cpp
	$(COMPILLER) -c $(FLAGS) battleobjects.cpp

interface.o: interface.cpp
	$(COMPILLER) -c $(FLAGS) interface.cpp

parser.o: parser.cpp
	$(COMPILLER) -c $(FLAGS) parser.cpp

converter.o: converter.cpp
	$(COMPILLER) -c $(FLAGS) converter.cpp

clear:
	-rm -f *.o *.gch sea-battle
