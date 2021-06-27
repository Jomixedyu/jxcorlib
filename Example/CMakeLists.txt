
include_directories("${PROJECT_SOURCE_DIR}")

aux_source_directory(./ EXAMPLE_SRC_DIR)
add_library(Example ${EXAMPLE_SRC_DIR})