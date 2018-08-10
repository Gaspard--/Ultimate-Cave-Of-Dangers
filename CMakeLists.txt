cmake_minimum_required(VERSION 2.6)

project(Ultimate-Cave-Of-Dangers)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

set(HEADER_DIRECTORY "include")
set(SOURCE_DIRECTORY "source")

if (WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else()
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -W -fdiagnostics-color=always -Wall -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wdisabled-optimization -Wduplicated-cond -Wextra -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wno-sign-conversion -Wnoexcept -Wnull-dereference -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wstrict-aliasing=1 -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wno-unknown-pragmas -Wuseless-cast -Wno-parentheses -Wno-unused-local-typedefs")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif(WIN32)

message("Flags used: '${CMAKE_CXX_FLAGS}'")

file(GLOB_RECURSE SOURCES_FILE "${SOURCE_DIRECTORY}/*.cpp" "${SOURCE_DIRECTORY}/*.c")
file(GLOB_RECURSE HEADERS_FILE "${HEADER_DIRECTORY}/*.hpp" "${HEADER_DIRECTORY}/*.h")

add_executable(
  ${PROJECT_NAME}
  ${SOURCES_FILE}
  ${HEADERS_FILE}
)

if (WIN32)
  include_directories (
    "C:/SFML-2.4.2/include"
  )
endif(WIN32)

include_directories(
  ${HEADER_DIRECTORY}
)

if (WIN32)
  target_link_libraries(${PROJECT_NAME}
                        C:/SFML-2.4.2/build/lib/Release/sfml-audio.lib
                        C:/SFML-2.4.2/build/lib/Release/sfml-graphics.lib
                        C:/SFML-2.4.2/build/lib/Release/sfml-system.lib
                        C:/SFML-2.4.2/build/lib/Release/sfml-window.lib
  )
else()
  target_link_libraries(${PROJECT_NAME}
                        sfml-audio
			sfml-graphics
			sfml-system
			sfml-window
  )
endif(WIN32)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
