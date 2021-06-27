
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_compile_options("/utf-8")
endif()

add_library(JxCode.CoreLib.Extension "JsonSerializer.cpp")