add_metall_test_executable(attributed_object_directory_test attributed_object_directory_test.cpp)

add_metall_test_executable(bin_manager_test bin_manager_test.cpp)

add_metall_test_executable(bin_directory_test bin_directory_test.cpp)

add_metall_test_executable(multilayer_bitset_test multilayer_bitset_test.cpp)

add_metall_test_executable(chunk_directory_test chunk_directory_test.cpp)

add_metall_test_executable(manager_test manager_test.cpp)

add_metall_test_executable(snapshot_test snapshot_test.cpp)

add_metall_test_executable(copy_file_test copy_file_test.cpp)

find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    add_metall_test_executable(manager_multithread_test manager_multithread_test.cpp)
    target_link_libraries(manager_multithread_test PRIVATE OpenMP::OpenMP_CXX)
else()
    MESSAGE(STATUS "OpenMP is not found. Will not run multi-thread test.")
endif()

add_metall_test_executable(multimanager_test multimanager_test.cpp)

add_metall_test_executable(mmap_segment_storage_test mmap_segment_storage_test.cpp)

add_metall_test_executable(object_attribute_accessor_test object_attribute_accessor_test.cpp)