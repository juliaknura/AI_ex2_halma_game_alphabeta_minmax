add_executable(main
main.cpp)

add_executable(speedTest
speedTest.cpp)

add_library(game
GameStructures.cpp
Algorithms.cpp
Reader.cpp)

target_link_libraries(main
game)

target_link_libraries(speedTest
game)