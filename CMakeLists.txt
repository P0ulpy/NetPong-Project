cmake_minimum_required (VERSION 3.8)
set(CMAKE_TOOLCHAIN_FILE ./vcpkg/scripts/buildsystems/vcpkg.cmake)

project (NetPong-Project VERSION 0.1.0 DESCRIPTION "Net pong")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS system window graphics network audio CONFIG REQUIRED)

# Cmake tests
# include(CTest)
# enable_testing()

if(WIN32)
    add_compile_definitions(WINDOWS=1)
endif ()

if (UNIX AND NOT APPLE)
    add_compile_definitions(LINUX=1)
endif ()

if (APPLE)
    add_compile_definitions(APPLE=1)
endif ()

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
    add_compile_definitions(_DEBUG=1)
else()
    add_compile_definitions(NDEBUG=1)
endif(${CMAKE_BUILD_TYPE} MATCHES Debug)

add_executable (NetPong-Project "src/main.cpp" "src/main.h")

target_link_libraries(NetPong-Project sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

file(COPY src/Assets DESTINATION /) 