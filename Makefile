ecosim: obj/main.o obj/game.o obj/world.o obj/chunk_manager.o obj/entity.o obj/resource_manager.o
	g++ $^ -rdynamic -o $@ -lsfml-graphics -lsfml-window -lsfml-system -lSegFault -std=c++17

obj/main.o: src/main.cpp src/game.h
	g++ -c src/main.cpp -o $@ -std=c++17


obj/game.o: src/game.cpp src/game.h
	g++ -c src/game.cpp -o $@ -std=c++17

obj/world.o: src/world.cpp src/world.h src/entity.h
	g++ -c src/world.cpp -o $@ -std=c++17

obj/chunk_manager.o: src/chunk_manager.cpp src/chunk_manager.h
	g++ -c src/chunk_manager.cpp -o $@ -std=c++17

obj/entity.o: src/entity.cpp src/entity.h
	g++ -c src/entity.cpp -o $@ -std=c++17

obj/resource_manager.o: src/resource_manager.cpp src/resource_manager.h
	g++ -c src/resource_manager.cpp -o $@ -std=c++17

clean:
	rm -f ecosim obj/*.o
