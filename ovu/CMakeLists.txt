file(GLOB_RECURSE files_header "*.hpp" "*.h" "*.hxx")
file(GLOB_RECURSE files_impl "*.cpp" "*.c" "*.cxx" "*.cc")
file(GLOB_RECURSE files_inl "*.inl")

set(source_files ${files_header} ${files_impl} ${files_inl})

add_library(${PROJECT_NAME} ${source_files})
target_include_directories(${PROJECT_NAME} PUBLIC ${OVU_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OVU_DEPENDENCY_LIBRARIES})