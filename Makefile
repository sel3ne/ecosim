ecosim: obj/main.o obj/game.o obj/world.o obj/chunk_manager.o obj/entity.o
	g++ $^ -rdynamic -o $@ -lsfml-graphics -lsfml-window -lsfml-system -lSegFault

obj/main.o: src/main.cpp src/game.h
	g++ -c src/main.cpp -o $@


obj/game.o: src/game.cpp src/game.h
	g++ -c src/game.cpp -o $@

obj/world.o: src/world.cpp src/world.h src/entity.h
	g++ -c src/world.cpp -o $@

obj/chunk_manager.o: src/chunk_manager.cpp src/chunk_manager.h
	g++ -c src/chunk_manager.cpp -o $@

obj/entity.o: src/entity.cpp src/entity.h
	g++ -c src/entity.cpp -o $@

clean:
	rm -f ecosim obj/*.o
