cmake_minimum_required (VERSION 3.6)
project(DDiscord)

set(BUILD_SHARED_LIBS OFF)

# Enable position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (UNIX)
	set(USE_SYSTEM_CURL ON)
	set(CMAKE_USE_OPENSSL ON)
endif()

add_compile_definitions(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

add_subdirectory(sleepy-discord)
add_subdirectory(bot)