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

  vector<Group> result = extractor(path);
  vector<Element> left = {};
  vector<Element> right = {};

  int totalshortcuts = 0;
  for (Group g : result) {
    totalshortcuts += g.shortcuts.size();
  }
  int shortcutsUntilNow = 0;
  bool previousPushWasLeft = true;
  int halfShortcutsLen = totalshortcuts / 2;
  for (int i = 0; i < result.size(); i++) {
    Group g = result[i];
    vector<Element> children = vector<Element>();
    bool isFirstLeft = i == 0;
    bool isFirstRight =
        previousPushWasLeft && shortcutsUntilNow > halfShortcutsLen;
    bool isFirstInAColumn = isFirstLeft || isFirstRight;
    bool shouldAddSpace = !isFirstInAColumn;
    if (shouldAddSpace) {
      children.push_back(text(""));
    }
    children.push_back(hbox({
        text(g.name) | bgcolor(Color::Aquamarine1),
        filler(),
    }));
    for (Shortcut s : g.shortcuts) {
      Element shortcutElem = hbox({
          text(s.keys) | bold,
          paragraph(" " + s.description),
      });
      children.push_back(shortcutElem);
    }
    if (shortcutsUntilNow <= halfShortcutsLen) {
      left.push_back(vbox(children));
    } else {
      right.push_back(vbox(children));
      previousPushWasLeft = false;
    }
    shortcutsUntilNow += g.shortcuts.size();
  }
  Element document = hbox({
      vbox(left),
      text(" "),
      vbox(right),
  });

  Screen screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  std::cout << screen.ToString() << std::endl;

  return EXIT_SUCCESS;
}
