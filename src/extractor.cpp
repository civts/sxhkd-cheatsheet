#include "group.cpp"
#include "shortcut.cpp"
#include <fstream>

/// Parses the sxhkd file nd returns the result
vector<Group> parseShortcutsFile(string path) {
  // Open the file
  vector<Group> result = vector<Group>();
  ifstream sxhkdrcFile;
  sxhkdrcFile.open(path);
  if (sxhkdrcFile.is_open()) {
    const string COMMENT_SYMBOL = "#";
    const string NAME_PREFIX = COMMENT_SYMBOL + " ";
    uint NAME_PREFIX_LEN = NAME_PREFIX.length();
    bool lineBeforeWasOnlyComment = false;
    bool readingGroupName = false;
    bool readingMultiline = false;
    bool isFirstGroup = true;
    bool isFirstShortcutOfGroup = true;
    string currentLine;
    Group currentGroup;
    Shortcut currentShortcut;
    // For each line in the file
    while (getline(sxhkdrcFile, currentLine)) {
      bool lineStartsWithCommentSymbol =
          currentLine.rfind(COMMENT_SYMBOL, 0) == 0;
      if (currentLine == COMMENT_SYMBOL) { // Either start or end of a group
                                           // name
        lineBeforeWasOnlyComment = true;
        if (readingGroupName) {
          readingGroupName = false;
        }
        continue;
      } else if (lineStartsWithCommentSymbol) {
        // Either the group name or a command comment
        string namePortion = currentLine.substr(NAME_PREFIX_LEN);
        if (lineBeforeWasOnlyComment) {
          // Finalize last group and create a new one
          if (!isFirstGroup) {
            currentGroup.shortcuts.push_back(currentShortcut);
            result.push_back(currentGroup);
            currentShortcut = Shortcut();
            isFirstShortcutOfGroup = true;
          }
          isFirstGroup = false;
          currentGroup = Group();
          currentGroup.name = namePortion;
          readingGroupName = true;
        } else if (readingGroupName) {
          currentGroup.name += namePortion;
        } else { // We are in a new shortcut
          if (!isFirstShortcutOfGroup) {
            currentGroup.shortcuts.push_back(currentShortcut);
          }
          isFirstShortcutOfGroup = false;
          currentShortcut = Shortcut();
          currentShortcut.description = namePortion;
        }
      } else if (currentLine.length() > 0) {
        // A shortcut or the respective command
        if (currentLine[0] == ' ') {
          // Command
          do {
            currentLine = currentLine.substr(1);
          } while (currentLine[0] == ' ');
          // Update shorcut command
          if (readingMultiline) {
            currentShortcut.command += currentLine;
          } else {
            currentShortcut.command = currentLine;
          }
          // Prepare for next iteration
          if (currentLine[currentLine.length() - 1] == '\\') {
            readingMultiline = true;
          } else {
            readingMultiline = false;
          }
        } else {
          // Shortcut
          currentShortcut.keys = currentLine;
        }
      }
      lineBeforeWasOnlyComment = false;
    }
    currentGroup.shortcuts.push_back(currentShortcut);
    result.push_back(currentGroup);
    sxhkdrcFile.close();
  }
  return result;
}
