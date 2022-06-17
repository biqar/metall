// Copyright 2021 Lawrence Livermore National Security, LLC and other Metall Project Developers.
// See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

// This program shows how to use STL unordered_set with Metall

#include <iostream>
#include <metall/metall.hpp>
#include <cstring>
#include <stdint.h>

// Metall contains basic STL containers that use metall as their default allocators.
#include <metall/container/unordered_set.hpp>
using namespace std;
//using namespace metall;

#define MAX_NUM 10000000
const string SNAPSHOT_DIR = "/l/ssd/metall-uset/snapshot_";

int main() {
  int snap_id = 0;
  // data insertion
  // using uset_t = unordered_set<int64_t, metall::allocator<int64_t>>;
  using uset_t = metall::container::unordered_set<int64_t>;
  {
    metall::manager metall_mgr(metall::create_only, "/l/ssd/metall-uset/metall");

    // Allocate a set object, passing an allocator object
    uset_t *puset = metall_mgr.construct<uset_t>("uset")(metall_mgr.get_allocator<int64_t>());

    auto start = std::chrono::high_resolution_clock::now();
    for(int64_t i=0; i<MAX_NUM; i+=1) {
      puset->insert(i);

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
    cout << "time taken to insert " << MAX_NUM << " items in the unordered_set: " << total_time << " ns." << endl;
  }

  // data retrival from unordered_set
  {
    metall::manager metall_mgr(metall::open_only, "/l/ssd/metall-uset/metall");

    auto *puset = metall_mgr.find<uset_t>("uset").first;
    std::cout << "# of Snapshots = " << snap_id << std::endl;
    std::cout << "Size = " << puset->size() << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for(int64_t i=0; i<MAX_NUM; i+=1) {
      assert(puset->find(i) != puset->end() && "Data missmatch in the unordered_set! Abort!");
    }
    auto end = std::chrono::high_resolution_clock::now();

    double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    cout << "time taken to retrieve " << MAX_NUM << " items from the unordered_set: " << total_time << " ns." << endl;
  }

  // data retrival from snapshots
  {
    for(int snapi=0; snapi<snap_id; snapi+=1) {
      cout << "opening snapshot " << snapi << std::endl;
      metall::manager metall_mgr(metall::open_only, (SNAPSHOT_DIR + to_string(snapi)).c_str());

      auto *puset = metall_mgr.find<uset_t>("uset").first;
      std::cout << "Size = " << puset->size() << std::endl;

      auto start = std::chrono::high_resolution_clock::now();
      for(int64_t i=0; i<puset->size(); i+=1) {
        assert(puset->find(i) != puset->end() && "Data missmatch in the unordered_set! Abort!");
      }
      auto end = std::chrono::high_resolution_clock::now();

      double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
      cout << "time taken to retrieve " << puset->size() << " items from the snapshot " << snapi << ": " << total_time << " ns." << endl;
    }
  }

  // destructor
  {
    metall::manager metall_mgr(metall::open_read_only, "/l/ssd/metall-uset/metall");
    auto puset = metall_mgr.find<uset_t>("set").first;

    metall_mgr.destroy_ptr(puset);// Destruct the object and deallocate memory
  }

  return 0;
}