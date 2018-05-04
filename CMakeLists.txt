cmake_minimum_required(VERSION 3.6)
project(NoName)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(SFML 2 COMPONENTS graphics window system audio)
find_package(Threads REQUIRED)

include_directories(${SFML_INCLUDE_DIR})

file(GLOB_RECURSE SOURCE_FILES
        "src/*.cpp"
        "src/*.h"
        )

add_executable(NoName ${SOURCE_FILES})

target_link_libraries(NoName ${SFML_LIBRARIES} Threads::Threads)

file(COPY res/ DESTINATION res/)
file(COPY settings.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/saves/)
