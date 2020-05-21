ecosim: obj/main.o obj/game.o obj/world.o obj/chunk_manager.o
	g++ $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system

obj/main.o: src/main.cpp
	g++ -c src/main.cpp -o $@


obj/game.o: src/game.cpp src/game.h
	g++ -c src/game.cpp -o $@

obj/world.o: src/world.cpp src/world.h
	g++ -c src/world.cpp -o $@

obj/chunk_manager.o: src/chunk_manager.cpp src/chunk_manager.h
	g++ -c src/chunk_manager.cpp -o $@