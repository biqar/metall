add_metall_executable(generate_rmat_edge_list generate_rmat_edge_list.cpp)
if (OpenMP_CXX_FOUND)
    target_link_libraries(generate_rmat_edge_list PRIVATE OpenMP::OpenMP_CXX)
endif ()