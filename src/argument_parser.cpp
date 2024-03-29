#include "argument_parser.hpp"
#include <iostream>
#include <string>

optional<Option> match(const string arg, const vector<Option> options) {
  if (arg[0] != '-') {
    cerr << "Expected argument, but got " << arg[0] << endl;
    exit(1);
  }
  if (arg[1] == '-') {
    string argWithoutHyphens = arg.substr(2);
    for (Option o : options) {
      if (argWithoutHyphens == o.longForm) {
        return optional(o);
      }
    }
  } else {
    string argWithoutHyphens = arg.substr(1);
    if (argWithoutHyphens.length() == 1) {
      for (Option o : options) {
        if (argWithoutHyphens[0] == o.shortForm) {
          return optional(o);
        }
      }
    }
  }
  return optional<Option>();
}

optional<Color> parsePalette16Color(string code) {
  if (code == "Black") {
    return Color::Black;
  } else if (code == "Red") {
    return Color::Red;
  } else if (code == "Green") {
    return Color::Green;
  } else if (code == "Yellow") {
    return Color::Yellow;
  } else if (code == "Blue") {
    return Color::Blue;
  } else if (code == "Magenta") {
    return Color::Magenta;
  } else if (code == "Cyan") {
    return Color::Cyan;
  } else if (code == "GrayLight") {
    return Color::GrayLight;
  } else if (code == "GrayDark") {
    return Color::GrayDark;
  } else if (code == "RedLight") {
    return Color::RedLight;
  } else if (code == "GreenLight") {
    return Color::GreenLight;
  } else if (code == "YellowLight") {
    return Color::YellowLight;
  } else if (code == "BlueLight") {
    return Color::BlueLight;
  } else if (code == "MagentaLight") {
    return Color::MagentaLight;
  } else if (code == "CyanLight") {
    return Color::CyanLight;
  } else if (code == "White") {
    return Color::White;
  }
  return optional<Color>();
}

ParsedArgs parseArgs(const int argc, const char *argv[]) {
  if (argc < 2) {
    cerr << "Missing configuration file path. See --help for details." << endl;
    exit(1);
  }
  const vector<Option> options = vector<Option>({
      {
          'c',
          "color",
          "  -c, --color                "
          "The color that will be used to highlight the \n"
          "                             "
          "background of the shortcut groups' names. If you \n"
          "                             "
          "pass this argument multiple times, the colors will\n"
          "                             "
          "be used sequentially (and repeated if needed) in \n"
          "                             "
          "the same order in which you passed them.\n\n"
          "                             "
          "The colors can be either in the hex format #RRGGBB\n"
          "                             "
          "(the initial # is optional) or one of the\n"
          "                             "
          "following: ['Black', 'Red', 'Green', 'Yellow',\n"
          "                             "
          "'Blue', 'Magenta', 'Cyan', 'GrayLight', 'GrayDark',\n"
          "                             "
          "'RedLight', 'GreenLight', 'YellowLight',\n"
          "                             "
          "'BlueLight', 'MagentaLight', 'CyanLight', White'].\n\n"
          "                             "
          "If no '-color' option is passed, the output will \n"
          "                             "
          "use as default color #16A085.",
      },
      {
          'h',
          "help",
          "  -h, --help                 "
          "Displays this help message.",
      },
      {
          't',
          "textColor",
          "  -t, --textColor            "
          "The color of the text that will be printed. See  \n"
          "                             "
          "'--color' for more info on the format. By default,\n"
          "                             "
          "it uses the default text color of your terminal.",
      },
  });
  vector<Color> colors = vector<Color>();
  Color textColor = Color::Default;
  for (int i = 1; i < argc - 1; i++) {
    string arg = argv[i];
    optional<Option> optMatch = match(arg, options);
    if (optMatch.has_value()) {
      Option matched = optMatch.value();
      if (argc - 1 <= ++i && (matched.shortForm != 'h')) {
        cerr << "Missing value for attribute " << arg
             << ". Use --help to get more details." << endl;
      }
      string value = argv[i];
      switch (matched.shortForm) {
      case 't':
      case 'c': {
        try {
          Color color;
          optional<Color> palette16Color = parsePalette16Color(value);
          if (palette16Color.has_value()) {
            color = palette16Color.value();
          } else {
            if (value[0] == '#') {
              value = value.substr(1);
            }
            if (value.length() != 6) {
              cerr << "'#" << value
                   << "' does not appear to be a valid color in the format "
                      "#RRGGBB"
                   << endl;
              exit(1);
            }
            string hexR = value.substr(0, 2);
            string hexG = value.substr(2, 2);
            string hexB = value.substr(4, 2);
            uint8_t r = stoi(hexR, nullptr, 16);
            uint8_t g = stoi(hexG, nullptr, 16);
            uint8_t b = stoi(hexB, nullptr, 16);
            color = Color::RGB(r, g, b);
          }
          if (matched.shortForm == 'c') {
            colors.push_back(color);
          } else {
            textColor = color;
          }
        } catch (invalid_argument _) {
          cerr << "'" << value
               << "' does not appear to be a valid color in the format #RRGGBB"
               << endl;
        }
        break;
      }
      case 'h': {
        cout << "sxhkd-cheatsheet version 1.0.0, (c) 2022 civts" << endl
             << endl
             << "Usage: sxhkd-cheatsheet [OPTIONS] [FILE]" << endl
             << "Sample usage:" << endl
             << "\tsxhkd-cheatsheet .config/sxhkd/sxhkdrc" << endl
             << "\tsxhkd-cheatsheet --color #2AA69A --color #E64A19 "
                ".config/sxhkd/sxhkdrc"
             << endl
             << endl
             << "Possible arguments:" << endl;
        for (Option o : options) {
          cout << o.helpMessage << endl << endl;
        }
        exit(0);
      }
      default: {
        cerr << "Well, it seems there is a bug in this code. Please open an "
                "issue on github providing the input that generated this "
                "message."
             << endl;
      }
      }
    } else {
      cerr << "Unrecognized argument in position " << i << ": " << arg
           << ". See --help for details";
      exit(1);
    }
  }
  string filepath = argv[argc - 1];
  filesystem::path p{filepath};
  if (!filesystem::exists(p)) {
    cerr
        << "Can not find the sxhkdrc file at '" << filepath
        << "'. Check the file existence or use --help to get the proper syntax."
        << endl;
    exit(1);
  }
  if (colors.size() == 0) {
    colors.push_back(Color::RGB(0x16, 0xA0, 0x85));
  }
  return {
      colors,
      textColor,
      filepath,
  };
}
