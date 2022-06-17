#include "argument_parser.hpp"
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
  ParsedArgs args = parseArgs(argc, argv);
  vector<Group> groups = parseShortcutsFile(args.filePath);
  displayShortcuts(groups, args);
  return EXIT_SUCCESS;
}
