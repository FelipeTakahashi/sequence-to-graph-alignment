cmake_minimum_required(VERSION 3.14)
project(gsap_solver CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3")

find_package(OpenMP REQUIRED)

add_library(gsap_graph src/graph/SequenceGraph.cpp)
target_include_directories(gsap_graph PUBLIC include)

add_library(gsap_io src/utils/Parser.cpp)
target_include_directories(gsap_io PUBLIC include)
target_link_libraries(gsap_io PUBLIC gsap_graph)

add_library(gsap_eval src/utils/HammingDistance.cpp)
target_include_directories(gsap_eval PUBLIC include)

add_library(gsap_heuristic
    src/heuristic/Greedy.cpp
    src/heuristic/BeamSearch.cpp
    src/heuristic/Brkga.cpp
)
target_include_directories(gsap_heuristic PUBLIC include)
target_link_libraries(gsap_heuristic PUBLIC gsap_graph)

if(OpenMP_CXX_FOUND)
  target_link_libraries(gsap_heuristic PUBLIC OpenMP::OpenMP_CXX)
endif()

enable_testing()

add_executable(test_graph tests/test_graph.cpp)
target_link_libraries(test_graph PRIVATE gsap_graph)
add_test(NAME GraphConstructionTest COMMAND test_graph)

add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser PRIVATE gsap_io gsap_graph)
add_test(NAME ParserTest COMMAND test_parser)

add_executable(test_distance tests/test_distance.cpp)
target_link_libraries(test_distance PRIVATE gsap_eval gsap_graph)
add_test(NAME DistanceTest COMMAND test_distance)

add_executable(test_greedy tests/test_greedy.cpp)
target_link_libraries(test_greedy PRIVATE gsap_io gsap_graph gsap_heuristic)
add_test(NAME GreedyHeuristicTest COMMAND test_greedy)

add_executable(test_beam tests/test_beam.cpp)
target_link_libraries(test_beam PRIVATE gsap_io gsap_graph gsap_heuristic)
add_test(NAME BeamSearchHeuristicTest COMMAND test_beam)

add_executable(test_brkga tests/test_brkga.cpp)
target_link_libraries(test_brkga PRIVATE gsap_io gsap_graph gsap_heuristic)
add_test(NAME BrkgaHeuristicTest COMMAND test_brkga)

add_executable(gsap_solver src/main.cpp)
target_link_libraries(gsap_solver PRIVATE gsap_io gsap_graph gsap_heuristic)
