cmake_minimum_required(VERSION 3.17)

project(ecosim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-g -Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic -lSegFault")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

set(
	SRCS
	src/building.h
  src/building.cpp
  src/chunk.cpp
  src/chunk.h
  src/chunk_manager.cpp
  src/chunk_manager.h
  src/constructible.cpp
  src/constructible.h
  src/delivery.cpp
  src/delivery.h
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
  src/map_generator.cpp
  src/map_generator.h
  src/resources.cpp
  src/resources.h
  src/resource_manager.cpp
  src/resource_manager.h
  src/settings.h
  src/tile.cpp
  src/tile.h
  src/ui.cpp
  src/ui.h
  src/vector_util.cpp
  src/vector_util.h
  src/world.cpp
  src/world.h)

add_executable(ecosim ${SRCS} src/main.cpp)
target_link_libraries(ecosim sfml-graphics sfml-audio)

# Testing with googletest

enable_testing()
include(GoogleTest)
add_executable(
	ecosim_tests
	${SRCS}
	src/game_test.cpp)
target_link_libraries(ecosim_tests gtest gtest_main sfml-graphics sfml-audio)
gtest_discover_tests(ecosim_tests WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
