// Copyright 2021 Lawrence Livermore National Security, LLC and other Metall Project Developers.
// See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <iostream>
#include <string>
#include <cstddef>

#include <metall/metall.hpp>
#include "../bench_driver.hpp"
#include "../../data_structure/multithread_adjacency_list.hpp"

using namespace adjacency_list_bench;

using key_type = uint64_t;
using value_type = uint64_t;
using adjacency_list_type =  data_structure::multithread_adjacency_list<key_type, value_type,
typename metall::manager::allocator_type<std::byte>>;

int main(int argc, char *argv[]) {

  bench_options option;
  if (!parse_options(argc, argv, &option)) {
    std::abort();
  }

  if (option.datastore_path_list.empty()) {
    std::cerr << "Out file name is required" << std::endl;
    std::abort();
  }

  {
    metall::manager manager(metall::open_only, option.datastore_path_list[0].c_str());
    manager.destroy<adjacency_list_type>(option.adj_list_key_name.c_str());

    if (!manager.all_memory_deallocated()) {
      std::cerr << "Some memory have not been freed." << std::endl;
      std::abort();
    }
  }

  return 0;
}