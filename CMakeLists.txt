cmake_minimum_required(VERSION 3.10)

project(dot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_compile_definitions(DOT_EXPORTS)
add_library(dot SHARED
	include/dot/public.h
	include/dot/type.h
	include/dot/object.h
	include/dot/box.h
	include/dot/rope.h
	include/dot/stdfwd.h
	include/dot/string.h
	include/dot/trace.h
	include/dot/fail.h
	include/dot/test.h
	sources/type.cpp
	sources/object.cpp
	sources/box.cpp
	sources/rope.cpp
	sources/string.cpp
	sources/trace.cpp
	sources/fail.cpp
	sources/test.cpp
)

remove_definitions(-DDOT_EXPORTS)
add_executable(test_dot
	tests/test_dot.cpp
	tests/test_object.cpp
	tests/test_box.cpp
	tests/test_rope.cpp
)

target_link_libraries(test_dot dot)
