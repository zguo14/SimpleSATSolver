cmake_minimum_required(VERSION 2.8.12)

project(bonus)

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

# create the main executable
## change .cpp files if needed
# add_executable(${PROJECT_NAME} main.cc)


# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# expose minisat header files
# include_directories(${CMAKE_SOURCE_DIR}/minisat)

# create the main executable
## change .cpp files if needed
add_executable(${PROJECT_NAME} main.cc tseitinTransformer.cc satSolver.cc treeNode.cc parser.cc tokenizer.cc)
# link MiniSAT libraries
# target_link_libraries(${PROJECT_NAME} minisat-lib-static)

# create the executable for tests
add_executable(tests test.cpp tseitinTransformer.cc satSolver.cc treeNode.cc parser.cc tokenizer.cc)
# link MiniSAT libraries
# target_link_libraries(tests minisat-lib-static)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

