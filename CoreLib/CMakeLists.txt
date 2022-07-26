
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_compile_options("/utf-8")
endif()

add_library(JxCode.CoreLib "UString.cpp" "istring.cpp" "Object.cpp" "Type.cpp" "Assembly.cpp" "Reflection.cpp" "File.cpp" "ManagedVector.hpp" "Converter.hpp" "Core.h"  "EnumUtil.h" "IInterface.h" "Delegate.h")