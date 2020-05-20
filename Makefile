ecosim: main.o
	g++ main.o -o ecosim -lsfml-graphics -lsfml-window -lsfml-system	

main.o: main.cpp
	g++ -c main.cpp


