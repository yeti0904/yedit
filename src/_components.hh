#pragma once

// C headers
#include <cstdint>
#include <unistd.h>
#include <sys/sysinfo.h>

// C++ headers
#include <string>
#include <vector>
using std::string;
using std::vector;
using std::to_string;

// C libraries
#include <ncurses.h>

// macros
#define ctrl(x)    ((x) & 0x1f)