#include "_components.hh"
#include "editor.hh"
#include "constants.hh"
#include "colourpairs.hh"

const string currentTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);
	return buf;
}

void Editor::Render(string statusbar, vector <string>& fbuf, size_t scrollY, size_t curx, size_t cury) {
	struct sysinfo info;
	sysinfo(&info);

	// render title bar
	attron(COLOR_PAIR(COLOUR_PAIR_BAR));
	mvhline(0, 0, ' ', COLS);
	mvprintw(0, 0, APP_NAME);
	attroff(COLOR_PAIR(COLOUR_PAIR_BAR));

	// render title bar info
	attron(COLOR_PAIR(COLOUR_PAIR_TIME));
	move(0, COLS - currentTime().length());
	addstr(currentTime().c_str());
	attroff(COLOR_PAIR(COLOUR_PAIR_TIME));

	attron(COLOR_PAIR(COLOUR_PAIR_MEM));
	string mem;
	// get memory usage percentage
	mem = to_string((int)((((double) info.totalram - (double) info.freeram) / info.totalram) * 100)) + "%";
	move(0, COLS - mem.length() - currentTime().length());
	addstr(mem.c_str());
	attroff(COLOR_PAIR(COLOUR_PAIR_MEM));

	// render editor
	attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));
	for (int i = 1; i < LINES - 1; ++i) {
		mvhline(i, 0, ' ', COLS);
	}

	// render editor contents
	for (size_t i = 0; i < fbuf.size(); ++i) {
		if (i + scrollY < fbuf.size()) {
			move(i + 1, 0);
			addstr(fbuf[i + scrollY].c_str());
		}
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