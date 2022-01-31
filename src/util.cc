#include "util.hh"

int Util::StringToColour(string colour) {
	if (colour == "black") {
		return COLOR_BLACK;
	}
	else if (colour == "red") {
		return COLOR_RED;
	}
	else if (colour == "yellow") {
		return COLOR_YELLOW;
	}
	else if (colour == "green") {
		return COLOR_GREEN;
	}
	else if (colour == "blue") {
		return COLOR_BLUE;
	}
	else if (colour == "magenta") {
		return COLOR_MAGENTA;
	}
	else if (colour == "cyan") {
		return COLOR_CYAN;
	}
	else if (colour == "white") {
		return COLOR_WHITE;
	}
	else if (colour == "grey") {
		return COLOR_GREY;
	}
	else if (colour == "lightred") {
		return COLOR_LIGHTRED;
	}
	else if (colour == "lightgreen") {
		return COLOR_LIGHTGREEN;
	}
	else if (colour == "lightyellow") {
		return COLOR_LIGHTYELLOW;
	}
	else if (colour == "lightblue") {
		return COLOR_LIGHTBLUE;
	}
	else if (colour == "lightmagenta") {
		return COLOR_LIGHTMAGENTA;
	}
	else if (colour == "lightcyan") {
		return COLOR_LIGHTCYAN;
	}
	else if (colour == "lightwhite") {
		return COLOR_LIGHTWHITE;
	}
	return COLOR_BLACK;
}