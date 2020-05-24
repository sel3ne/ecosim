cmake_minimum_required(VERSION 3.17)

project(ecosim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic -lSegFault")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(
  ecosim
  src/building.h
  src/building.cpp
  src/chunk.cpp
  src/chunk.h
  src/chunk_manager.cpp
  src/chunk_manager.h
  src/constructible.cpp
  src/constructible.h
  src/entity.cpp
  src/entity.h
  src/farmhouse.cpp
  src/farmhouse.h
  src/game.cpp
  src/game.h
  src/grid.cpp
  src/grid.h
  src/human.cpp
  src/human.h
  src/main.cpp
  src/map_generator.cpp
  src/map_generator.h
  src/resource_manager.cpp
  src/resource_manager.h
  src/settings.h
  src/tile.cpp
  src/tile.h
  src/ui.cpp
  src/ui.h
  src/world.cpp
  src/world.h)

target_link_libraries(ecosim sfml-graphics sfml-audio)
