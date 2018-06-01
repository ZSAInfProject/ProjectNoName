cmake_minimum_required(VERSION 3.6)
project(NoName)

set(CMAKE_CXX_STANDARD 14)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
		${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
	)
ENDIF()


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
file(COPY src/gui/themes.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/saves/)
file(MAKE_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/saves/)
