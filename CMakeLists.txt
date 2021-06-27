
cmake_minimum_required (VERSION 3.8)

project ("JxCoreLibTest")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_compile_options("/utf-8")
endif()
set(CMAKE_CXX_STANDARD 20)

include_directories(".")

add_subdirectory("./CoreLib")
add_subdirectory("./CoreLib.Extension")
add_subdirectory("./Example")

aux_source_directory(./ DIR_SRCS)

add_executable (${PROJECT_NAME} ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME} "JxCode.CoreLib")
target_link_libraries(${PROJECT_NAME} "JxCode.CoreLib.Extension")
target_link_libraries(${PROJECT_NAME} "Example")
