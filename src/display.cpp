#include "argument_parser.cpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "group.cpp"
#include "shortcut.cpp"
#include <csignal>
#include <iostream>

using namespace ftxui;
using namespace std;

Element obtainElement(const vector<Group> groups, const ParsedArgs args) {
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
          text(s.keys + " ") | bold,
          paragraph(s.description),
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
  return document;
}

Component componentPtr;
void handleResize(int code) {
  if (code != SIGWINCH) {
    return;
  }
  // Trigger an event on the ScreenInteractive so that it resizes
  componentPtr->OnEvent(Event::Custom);
}

void showOnScreen(Element document) {
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  Component renderer = Renderer([&] { return document | center; });
  Component component = CatchEvent(renderer, [&](Event event) {
    screen.Clear();
    bool didUserPressQ = event == Event::Character('q');
    bool didUserPressEsc = event == Event::Escape;
    if (didUserPressQ || didUserPressEsc) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });
  componentPtr = component;

  struct sigaction sa;
  sa.sa_handler = handleResize;
  sa.sa_flags = SA_RESTART;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGWINCH, &sa, nullptr);

  screen.Loop(component);
}

void displayShortcuts(const vector<Group> groups, const ParsedArgs args) {
  Element document = obtainElement(groups, args);
  showOnScreen(document);
}
