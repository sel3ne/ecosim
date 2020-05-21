ecosim: obj/main.o obj/game.o
	g++ $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system

obj/main.o: src/main.cpp
	g++ -c src/main.cpp -o $@


obj/game.o: src/game.cpp src/game.h
	g++ -c src/game.cpp -o $@
