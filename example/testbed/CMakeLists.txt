file(GLOB_RECURSE files_header "*.hpp" "*.h" "*.hxx")
file(GLOB_RECURSE files_impl "*.cpp" "*.c" "*.cxx" "*.cc")
file(GLOB_RECURSE files_inl "*.inl")

set(source_files ${files_header} ${files_impl} ${files_inl})

add_executable(${PROJECT_NAME}_Testbed ${source_files})
target_include_directories(${PROJECT_NAME}_Testbed PUBLIC ${OVU_ALL_INCLUDES} include)
target_link_libraries(${PROJECT_NAME}_Testbed ${OVU_ALL_LIBRARIES})