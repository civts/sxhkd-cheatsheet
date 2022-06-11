#ifndef group
#define group
#include "shortcut.cpp"
#include <string>
#include <vector>

using namespace ::std;

typedef struct {
  vector<Shortcut> shortcuts = vector<Shortcut>();
  string name;
} Group;

#endif
