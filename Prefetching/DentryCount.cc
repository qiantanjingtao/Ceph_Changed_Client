#include "DentryCount.h"
#include <iostream>


void DentryCount::push_back(Dentry* den) {   // cannot make sure which function to add
  Mutex::Locker lock(dentry_count_lock);
  assert(den);
  string sden = dentry_to_string(den);
  if (dentry_list.count(sden) == 0) {
     list<utime_t> lden;
     lden.push_back(ceph_clock_now());
     dentry_list[dentry_to_string(den)].first = false;  // 初始化为false
     dentry_list[dentry_to_string(den)].second = lden;
  } else {
     dentry_list[sden].second.push_back(ceph_clock_now());
  }
  
  _size++;
}

void DentryCount::new_dentry_list(Dentry* den) {  // new dentry list for map
  Mutex::Locker lock(dentry_count_lock);
  assert(den);
  list<utime_t> lden;
  lden.push_back(ceph_clock_now());
  dentry_list[dentry_to_string(den)].first = false;  // 初始化为false
  dentry_list[dentry_to_string(den)].second = lden;
  
  _size++;
}

void DentryCount::add_single_dentry(Dentry* den) {  // add one dentry to current lis
  Mutex::Locker lock(dentry_count_lock);
  assert(den);
  dentry_list[dentry_to_string(den)].second.push_back(ceph_clock_now());

  _size++;
}

void DentryCount::set_prefetch_flag(Dentry* den) {  // add one dentry to current lis
  Mutex::Locker lock(dentry_count_lock);
  assert(den);
  dentry_list[dentry_to_string(den)].first = true;
}

bool DentryCount::get_prefetch_flag(Dentry* den) {
  Mutex::Locker lock(dentry_count_lock);
  assert(den);
  return dentry_list[dentry_to_string(den)].first;
}

void DentryCount::display() {
    auto map_it = dentry_list.cbegin();

    while (map_it != dentry_list.cend()) {
        auto internal_it = map_it->second.second.begin();

        while (internal_it != map_it->second.second.end()) {
              std::cout << *internal_it;
              internal_it++;
        }
        map_it++;
    }

}

void DentryCount::erase(Dentry* den) {
    Mutex::Locker lock(dentry_count_lock);
    dentry_list.erase(dentry_to_string(den));
}

size_t DentryCount::get_list_size(Dentry* den) {
  string id = dentry_to_string(den);
  return dentry_list[id].second.size();
}
