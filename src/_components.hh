#pragma once

// C headers
#include <cstdint>
#include <unistd.h>
#include <sys/sysinfo.h>

// C++ headers
#include <string>
#include <vector>
#include <fstream>
using std::string;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::to_string;

// C libraries
#include <ncurses.h>

// macros
#define ctrl(x)    ((x) & 0x1f)

// colours
#define COLOR_GREY         8
#define COLOR_LIGHTRED     9
#define COLOR_LIGHTGREEN   10
#define COLOR_LIGHTYELLOW  11
#define COLOR_LIGHTBLUE    12
#define COLOR_LIGHTMAGENTA 13
#define COLOR_LIGHTCYAN    14
#define COLOR_LIGHTWHITE   15