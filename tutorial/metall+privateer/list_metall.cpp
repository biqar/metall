// Copyright 2021 Lawrence Livermore National Security, LLC and other Metall Project Developers.
// See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

// This program shows how to use STL list with Metall

#include <iostream>
#include <metall/metall.hpp>
#include <cstring>
#include <stdint.h>

// Metall contains basic STL containers that use metall as their default allocators.
#include <metall/container/list.hpp>
#include <boost/foreach.hpp>
using namespace std;
//using namespace metall;

#define MAX_NUM 10000000
const string SNAPSHOT_DIR = "/l/ssd/privateer-list/snapshot_";

int main() {
  int snap_id = 0;
  // data insertion
  // using list_t = list<int64_t, metall::allocator<int64_t>>;
  using list_t = metall::container::list<int64_t>;
  {
    metall::manager metall_mgr(metall::create_only, "/l/ssd/privateer-list/metall");

    // Allocate a list object, passing an allocator object
    list_t *plist = metall_mgr.construct<list_t>("list")(metall_mgr.get_allocator<int64_t>());

    auto start = std::chrono::high_resolution_clock::now();
    for(int64_t i=0; i<MAX_NUM; i+=1) {
      plist->push_back(i);

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
    cout << "time taken to insert " << MAX_NUM << " items in the list: " << total_time << " ns." << endl;
  }

  // data retrival from list
  {
    metall::manager metall_mgr(metall::open_only, "/l/ssd/privateer-list/metall");

    auto *plist = metall_mgr.find<list_t>("list").first;
    std::cout << "# of Snapshots = " << snap_id << std::endl;
    std::cout << "Size = " << plist->size() << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    int64_t curr_idx = 0;
    BOOST_FOREACH(int64_t elem, (*plist)) {
      assert(elem == curr_idx && "Data missmatch in the list! Abort!");
      curr_idx += 1;
    }
    auto end = std::chrono::high_resolution_clock::now();

    double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    cout << "time taken to retrieve " << MAX_NUM << " items from the list: " << total_time << " ns." << endl;
  }

  // data retrival from snapshots
  {
    for(int snapi=0; snapi<snap_id; snapi+=1) {
      cout << "opening snapshot " << snapi << std::endl;
      metall::manager metall_mgr(metall::open_only, (SNAPSHOT_DIR + to_string(snapi)).c_str());

      auto *plist = metall_mgr.find<list_t>("list").first;
      std::cout << "Size = " << plist->size() << std::endl;

      auto start = std::chrono::high_resolution_clock::now();
      int64_t curr_idx = 0;
      BOOST_FOREACH(int64_t elem, (*plist)) {
          assert(elem == curr_idx && "Data missmatch in the list! Abort!");
          curr_idx += 1;
      }
      auto end = std::chrono::high_resolution_clock::now();

      double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
      cout << "time taken to retrieve " << plist->size() << " items from the snapshot " << snapi << ": " << total_time << " ns." << endl;
    }
  }

  // destructor
  {
    metall::manager metall_mgr(metall::open_read_only, "/l/ssd/privateer-list/metall");
    auto plist = metall_mgr.find<list_t>("list").first;

    metall_mgr.destroy_ptr(plist);// Destruct the object and deallocate memory
  }

  return 0;
}