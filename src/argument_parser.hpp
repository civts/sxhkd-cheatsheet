#ifndef arg_parser
#define arg_parser
#include "ftxui/screen/color.hpp"
#include <bits/stdint-uintn.h>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using namespace std;
using namespace ftxui;

typedef struct {
  char shortForm;
  string longForm;
  string helpMessage;
} Option;

typedef struct {
  vector<Color> colors = vector<Color>(Color::Aquamarine1);
  Color textColor;
  string filePath;
} ParsedArgs;

ParsedArgs parseArgs(const int argc, const char *argv[]);

optional<Option> match(const string arg, const vector<Option> options);

#endif
