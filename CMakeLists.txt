cmake_minimum_required(VERSION 3.6)
project(NoName)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SOURCE_FILES
        "src/*.cpp"
        "src/*.h"
        )

add_executable(NoName ${SOURCE_FILES})

set(SFML_BUILD_DOC FALSE)
set(SFML_BUILD_EXAMPLES FALSE)
set(BUILD_SHARED_LIBS FALSE)

include_directories(deps/SFML/include)
add_subdirectory(deps/SFML)

include_directories(deps/imgui-sfml)
add_subdirectory(deps/imgui-sfml)

include_directories(deps/SFGUI/include)
add_subdirectory(deps/SFGUI)

target_link_libraries(NoName sfml-network sfml-graphics sfml-window sfml-system sfml-audio sfgui imgui-sfml)

file(COPY res/ DESTINATION res/)
file(COPY settings.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/saves/)
file(MAKE_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/saves/)