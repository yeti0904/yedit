#include "iohandle.hh"
#include "colourpairs.hh"

void IOHandle::SetTermTitle(const string& title) {
	printf("\033]0;%s\007", title.c_str());
}

void IOHandle::Init() {
	// init ncurses
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	start_color();
	use_default_colors();
	nodelay(stdscr, true);
	init_pair(COLOUR_PAIR_EDITOR, COLOR_WHITE, COLOR_BLUE);
	init_pair(COLOUR_PAIR_BAR, COLOR_BLACK, COLOR_WHITE);
	init_pair(COLOUR_PAIR_CURSOR, COLOR_BLUE, COLOR_WHITE);
	init_pair(COLOUR_PAIR_TIME, COLOR_BLACK, COLOR_GREEN);
	init_pair(COLOUR_PAIR_MEM, COLOR_BLACK, COLOR_RED);
	init_pair(COLOUR_PAIR_ESC, COLOR_RED, COLOR_BLACK);
	init_pair(COLOUR_PAIR_ALERT, COLOR_BLACK, COLOR_GREEN);
}

void IOHandle::Exit() {
	endwin();
}
