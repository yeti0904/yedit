#pragma once

// C headers
#include <cstdint>
#include <unistd.h>
#include <dirent.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

// C++ headers
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <functional>
using std::ref;
using std::stoi;
using std::string;
using std::vector;
using std::thread;
using std::ofstream;
using std::ifstream;
using std::to_string;

// C libraries
#include <ncurses.h>

// C++ libraries
#include <dotproperties.hh>

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