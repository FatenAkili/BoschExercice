cmake_minimum_required(VERSION 3.10)
project(Exercice)


include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(exercice ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(exercice PRIVATE Threads::Threads)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_SRC "tests/*.cpp")

add_executable(runTests ${TEST_SRC})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
