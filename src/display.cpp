#include "argument_parser.cpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "group.cpp"
#include "shortcut.cpp"
#include <iostream>

using namespace ftxui;
using namespace std;

void displayShortcuts(const vector<Group> groups, const ParsedArgs args) {
  vector<Element> left = {};
  vector<Element> right = {};
  int totalshortcuts = 0;
  for (Group g : groups) {
    totalshortcuts += g.shortcuts.size();
  }
  int shortcutsUntilNow = 0;
  bool previousPushWasLeft = true;
  int halfShortcutsLen = totalshortcuts / 2;
  for (int i = 0; i < groups.size(); i++) {
    Group g = groups[i];
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
        text(" " + g.name + " ") | bgcolor(args.colors[i % args.colors.size()]),
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
                     }) |
                     color(args.textColor);

  Screen screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  std::cout << screen.ToString() << std::endl;
}
