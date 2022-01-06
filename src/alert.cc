#include "alert.hh"
#include "colourpairs.hh"

UI::Alert::Alert() {
	time = 0;
	text = "Empty Alert";
}

void UI::Alert::DoTime() {
	time -= 1000/60;
}

void UI::Alert::Render() {
	move((int) (LINES / 2), (int) (COLS / 2) - (int) (text.length() / 2) - 4);
	attron(COLOR_PAIR(COLOUR_PAIR_ALERT));
	printw("[ %s ]", text.c_str());
	attroff(COLOR_PAIR(COLOUR_PAIR_ALERT));
}