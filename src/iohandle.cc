#include "iohandle.hh"
#include "colourpairs.hh"
#include "util.hh"

void IOHandle::SetTermTitle(const string& title) {
	printf("\033]0;%s\007", title.c_str());
}

void IOHandle::Init(Properties theme) {
	// init ncurses
	initscr();
	raw();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	start_color();
	use_default_colors();
	nodelay(stdscr, true);
	init_pair(COLOUR_PAIR_EDITOR, Util::StringToColour(theme["editorFG"]), Util::StringToColour(theme["editorBG"]));
	init_pair(COLOUR_PAIR_BAR, Util::StringToColour(theme["barFG"]), Util::StringToColour(theme["barBG"]));
	init_pair(COLOUR_PAIR_CURSOR, COLOR_BLUE, COLOR_WHITE);
	init_pair(COLOUR_PAIR_TIME, Util::StringToColour(theme["timeFG"]), Util::StringToColour(theme["timeBG"]));
	init_pair(COLOUR_PAIR_MEM, COLOR_BLACK, COLOR_RED);
	init_pair(COLOUR_PAIR_ESC, COLOR_RED, COLOR_BLACK);
	init_pair(COLOUR_PAIR_ALERT, COLOR_BLACK, COLOR_GREEN);
}

void IOHandle::Exit() {
	endwin();
}
