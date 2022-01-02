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
	init_pair(COLOUR_PAIR_CURSOR, COLOR_WHITE, COLOR_BLUE);
}

void IOHandle::Exit() {
	endwin();
}
