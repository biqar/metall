// Copyright 2021 Lawrence Livermore National Security, LLC and other Metall Project Developers.
// See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

// This program shows how to use STL vector with Metall

#include <iostream>
#include <metall/metall.hpp>
#include <cstring>
#include <stdint.h>

// Metall contains basic STL containers that use metall as their default allocators.
#include <metall/container/vector.hpp>
using namespace std;
//using namespace metall;

#define MAX_NUM 10000000
const string SNAPSHOT_DIR = "/l/ssd/metall-vector/snapshot_";

int main() {
  int snap_id = 0;
  // data insertion
  // using vec_t = vector<int64_t, metall::allocator<int64_t>>;
  using vec_t = metall::container::vector<int64_t>;
  {
    metall::manager metall_mgr(metall::create_only, "/l/ssd/metall-vector/metall");

    // Allocate a vector object, passing an allocator object
    vec_t *pvec = metall_mgr.construct<vec_t>("vec")(metall_mgr.get_allocator<int64_t>());

    auto start = std::chrono::high_resolution_clock::now();
    for(int64_t i=0; i<MAX_NUM; i+=1) {
      pvec->push_back(i);

      // Create a snapshot
      if(i && i%1000000==0) {
        metall_mgr.snapshot((SNAPSHOT_DIR + to_string(snap_id)).c_str());
        snap_id += 1;
      }
    }

    // last snapshot
    metall_mgr.snapshot((SNAPSHOT_DIR + to_string(snap_id)).c_str());
    snap_id += 1;

    auto end = std::chrono::high_resolution_clock::now();

    double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    cout << "time taken to insert " << MAX_NUM << " items in the vector: " << total_time << " ns." << endl;
  }

  // data retrival from vector
  {
    metall::manager metall_mgr(metall::open_only, "/l/ssd/metall-vector/metall");

    auto *pvec = metall_mgr.find<vec_t>("vec").first;
    std::cout << "# of Snapshots = " << snap_id << std::endl;
    std::cout << "Size = " << pvec->size() << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for(int64_t i=0; i<MAX_NUM; i+=1) {
      assert((*pvec)[i] == i && "Data missmatch in the vector! Abort!");
    }
    auto end = std::chrono::high_resolution_clock::now();

    double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    cout << "time taken to retrieve " << MAX_NUM << " items from the vector: " << total_time << " ns." << endl;
  }

  // data retrival from snapshots
  {
    for(int snapi=0; snapi<snap_id; snapi+=1) {
      cout << "opening snapshot " << snapi << std::endl;
      metall::manager metall_mgr(metall::open_only, (SNAPSHOT_DIR + to_string(snapi)).c_str());

      auto *pvec = metall_mgr.find<vec_t>("vec").first;
      std::cout << "Size = " << pvec->size() << std::endl;

      auto start = std::chrono::high_resolution_clock::now();
      for(int64_t i=0; i<pvec->size(); i+=1) {
        assert((*pvec)[i] == i && "Data missmatch in the vector! Abort!");
      }
      auto end = std::chrono::high_resolution_clock::now();

      double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
      cout << "time taken to retrieve " << pvec->size() << " items from the snapshot " << snapi << ": " << total_time << " ns." << endl;
    }
  }

  // destructor
  {
    metall::manager metall_mgr(metall::open_read_only, "/l/ssd/metall-vector/metall");
    auto pvec = metall_mgr.find<vec_t>("vec").first;

    metall_mgr.destroy_ptr(pvec);// Destruct the object and deallocate memory
  }

  return 0;
}