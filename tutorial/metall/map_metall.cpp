// Copyright 2021 Lawrence Livermore National Security, LLC and other Metall Project Developers.
// See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

// This program shows how to use STL map with Metall

#include <iostream>
#include <metall/metall.hpp>
#include <cstring>
#include <stdint.h>

// Metall contains basic STL containers that use metall as their default allocators.
#include <metall/container/map.hpp>
using namespace std;
//using namespace metall;

#define MAX_NUM 10000000
const string SNAPSHOT_DIR = "/l/ssd/metall-map/snapshot_";

int main() {
  int snap_id = 0;
  // data insertion
  // using map_t = map<int64_t, metall::allocator<int64_t>>;
  using map_t = metall::container::map<int64_t, int64_t>;
  {
    metall::manager metall_mgr(metall::create_only, "/l/ssd/metall-map/metall");

    // Allocate a map object, passing an allocator object
    map_t *pmap = metall_mgr.construct<map_t>("map")(metall_mgr.get_allocator<int64_t>());

    auto start = std::chrono::high_resolution_clock::now();
    for(int64_t i=0; i<MAX_NUM; i+=1) {
      (*pmap)[i] = i;

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
    cout << "time taken to insert " << MAX_NUM << " items in the map: " << total_time << " ns." << endl;
  }

  // data retrival from map
  {
    metall::manager metall_mgr(metall::open_only, "/l/ssd/metall-map/metall");

    auto *pmap = metall_mgr.find<map_t>("map").first;
    std::cout << "# of Snapshots = " << snap_id << std::endl;
    std::cout << "Size = " << pmap->size() << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    for(int64_t i=0; i<MAX_NUM; i+=1) {
      assert((pmap->find(i) != pmap->end() && (*pmap)[i] == i) && "Data missmatch in the map! Abort!");
    }
    auto end = std::chrono::high_resolution_clock::now();

    double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    cout << "time taken to retrieve " << MAX_NUM << " items from the map: " << total_time << " ns." << endl;
  }

  // data retrival from snapshots
  {
    for(int snapi=0; snapi<snap_id; snapi+=1) {
      cout << "opening snapshot " << snapi << std::endl;
      metall::manager metall_mgr(metall::open_only, (SNAPSHOT_DIR + to_string(snapi)).c_str());

      auto *pmap = metall_mgr.find<map_t>("map").first;
      std::cout << "Size = " << pmap->size() << std::endl;

      auto start = std::chrono::high_resolution_clock::now();
      for(int64_t i=0; i<pmap->size(); i+=1) {
        assert((pmap->find(i) != pmap->end() && (*pmap)[i] == i) && "Data missmatch in the map! Abort!");
      }
      auto end = std::chrono::high_resolution_clock::now();

      double total_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
      cout << "time taken to retrieve " << pmap->size() << " items from the snapshot " << snapi << ": " << total_time << " ns." << endl;
    }
  }

  // destructor
  {
    metall::manager metall_mgr(metall::open_read_only, "/l/ssd/metall-map/metall");
    auto pmap = metall_mgr.find<map_t>("map").first;

    metall_mgr.destroy_ptr(pmap);// Destruct the object and deallocate memory
  }

  return 0;
}