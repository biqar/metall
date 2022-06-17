// Copyright 2021 Lawrence Livermore National Security, LLC and other Metall Project Developers.
// See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

// This program shows how to use STL deque with Metall

#include <iostream>
#include <metall/metall.hpp>
#include <cstring>
#include <stdint.h>

// Metall contains basic STL containers that use metall as their default allocators.
#include <metall/container/deque.hpp>
using namespace std;
//using namespace metall;

#define MAX_NUM 10000000
const string SNAPSHOT_DIR = "/l/ssd/metall-deque/snapshot_";

int main() {
  int snap_id = 0;
  // data insertion
  // using deque_t = deque<int64_t, metall::allocator<int64_t>>;
  using deque_t = metall::container::deque<int64_t>;
  {
    metall::manager metall_mgr(metall::create_only, "/l/ssd/metall-deque/metall");

    // Allocate a deque object, passing an allocator object
    deque_t *pdeque = metall_mgr.construct<deque_t>("deque")(metall_mgr.get_allocator<int64_t>());

    auto start = std::chrono::high_resolution_clock::now();
    for(int64_t i=0; i<MAX_NUM; i+=1) {
      pdeque->push_back(i);

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
    cout << "time taken to insert " << MAX_NUM << " items in the deque: " << total_time << " ns." << endl;
  }

  // data retrival from deque
  {
    metall::manager metall_mgr(metall::open_only, "/l/ssd/metall-deque/metall");

    auto *pdeque = metall_mgr.find<deque_t>("deque").first;
    std::cout << "# of Snapshots = " << snap_id << std::endl;
    std::cout << "Size = " << pdeque->size() << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for(int64_t i=0; i<MAX_NUM; i+=1) {
      assert((*pdeque)[i] == i && "Data missmatch in the deque! Abort!");
    }
    auto end = std::chrono::high_resolution_clock::now();

    double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    cout << "time taken to retrieve " << MAX_NUM << " items from the deque: " << total_time << " ns." << endl;
  }

  // data retrival from snapshots
  {
    for(int snapi=0; snapi<snap_id; snapi+=1) {
      cout << "opening snapshot " << snapi << std::endl;
      metall::manager metall_mgr(metall::open_only, (SNAPSHOT_DIR + to_string(snapi)).c_str());

      auto *pdeque = metall_mgr.find<deque_t>("deque").first;
      std::cout << "Size = " << pdeque->size() << std::endl;

      auto start = std::chrono::high_resolution_clock::now();
      for(int64_t i=0; i<pdeque->size(); i+=1) {
        assert((*pdeque)[i] == i && "Data missmatch in the deque! Abort!");
      }
      auto end = std::chrono::high_resolution_clock::now();

      double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
      cout << "time taken to retrieve " << pdeque->size() << " items from the snapshot " << snapi << ": " << total_time << " ns." << endl;
    }
  }

  // destructor
  {
    metall::manager metall_mgr(metall::open_read_only, "/l/ssd/metall-deque/metall");
    auto pdeque = metall_mgr.find<deque_t>("deque").first;

    metall_mgr.destroy_ptr(pdeque);// Destruct the object and deallocate memory
  }

  return 0;
}