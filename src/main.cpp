#include "display.cpp"
#include "extractor.cpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/screen.hpp"
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
  vector<Group> groups = parseShortcutsFile(path);
  displayShortcuts(groups);
  return EXIT_SUCCESS;
}
