#include "extractor.cpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "group.cpp"
#include "shortcut.cpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace ftxui;

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    cerr << "This program accepts only one positional argument: the path "
            "to the sxhkdrc file";
    exit(1);
  }
  string path = argv[1];
  cout << path << endl << endl;

  vector<Group> result = extractor(path);
  for (Group g : result) {
    cout << ">>>> " << g.name << endl;
    for (Shortcut s : g.shortcuts) {
      // cout << "-- " << s.keys << endl;
      cout << "- " << s.description << endl;
      // cout << "---- " << s.command << endl;
    }
  }
  return 0;
}
