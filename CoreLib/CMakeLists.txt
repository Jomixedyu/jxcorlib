
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_compile_options("/utf-8")
endif()

add_library(JxCode.CoreLib "UString.cpp" "Object.cpp" "Type.cpp" "CommonException.cpp" "Reflection.cpp" "File.cpp" "ManagedVector.h" "Converter.hpp" "Core.h")