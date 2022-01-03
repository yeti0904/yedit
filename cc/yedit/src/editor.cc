#include "_components.hh"
#include "editor.hh"
#include "constants.hh"
#include "colourpairs.hh"

void Editor::Render(string statusbar, vector <string>& fbuf, size_t scrollY, size_t curx, size_t cury) {
	// render title bar
	attron(COLOR_PAIR(COLOUR_PAIR_BAR));
	mvhline(0, 0, ' ', COLS);
	mvprintw(0, 0, APP_NAME);
	attroff(COLOR_PAIR(COLOUR_PAIR_BAR));

	// render editor
	attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));
	for (int i = 1; i < LINES - 1; i++) {
		mvhline(i, 0, ' ', COLS);
	}

	// render editor contents
	move(1, 0);
	for (size_t i = scrollY; i < fbuf.size(); i++) {
		printw(fbuf[i].c_str());
		addch('\n');
	}

	attroff(COLOR_PAIR(COLOUR_PAIR_EDITOR));

	// render cursor
	attron(COLOR_PAIR(COLOUR_PAIR_CURSOR));
	move(cury + 1 + scrollY, curx);
	if (curx >= fbuf[cury].length())
		addch(' ');
	else
		addch(fbuf[cury][curx]);
	attroff(COLOR_PAIR(COLOUR_PAIR_CURSOR));

	// render status bar
	attron(COLOR_PAIR(COLOUR_PAIR_BAR));
	mvhline(LINES - 1, 0, ' ', COLS);
	move(LINES - 1, 0);
	addstr(statusbar.c_str());
	attroff(COLOR_PAIR(COLOUR_PAIR_BAR));
}