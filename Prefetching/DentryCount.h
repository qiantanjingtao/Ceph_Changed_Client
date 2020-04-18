#include <utime.h>
#include <sstream>
#include <list>
#include <string>
#include <utility>
#include "Dentry.h"
#include "common/Mutex.h"

class DentryCount {
private:
  ceph::unordered_map <std::string, pair<bool, std::list<utime_t>>> dentry_list;
  size_t _size;
  Mutex   dentry_count_lock;
public:
  utime_t last_call;

public:
  DentryCount(ceph::unordered_map <std::string, std::list<utime_t>> dl) : _size(0), dentry_count_lock("dentry_count_lock"), last_call(ceph_clock_now()) {
      auto map_it = dl.cbegin();
      while(map_it != dl.cend()) {
         dentry_list[map_it->first] = make_pair(false, map_it->second);
         map_it++;
      }
  }

  DentryCount() : _size(0), dentry_count_lock("dentry_count_lock"), last_call(ceph_clock_now()) {}

  string dentry_to_string(Dentry* den) {
    ostringstream ost;
    ost << den;
    return ost.str() + den->name;
  }

  void push_back(Dentry* den);
  void new_dentry_list(Dentry* den);
  void add_single_dentry(Dentry* den);
  void display();
  void erase(Dentry *den);
  void set_prefetch_flag(Dentry* den);
  bool get_prefetch_flag(Dentry* den);
  size_t get_list_size(Dentry* den);
  size_t get_size() {
    return _size;
  }
};
