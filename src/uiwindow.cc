#include "uiwindow.hh"

void UI::Window::Render() {
	attron(COLOR_PAIR(colour_pair));
	// draw background of window
	for (uint16_t i = 0; i < h; i++) {
		mvhline(y + i, x, ' ', w);
	}

	// draw border
	move(y, x);
	addch(ACS_ULCORNER);
	mvhline(y, x + 1, ACS_HLINE, w - 2);
	move(y + h - 1, x);
	addch(ACS_LLCORNER);
	mvhline(y + h - 1, x + 1, ACS_HLINE, w - 2);
	mvvline(y + 1, x, ACS_VLINE, h - 2);
	move(y, x + w - 1);
	addch(ACS_URCORNER);
	mvvline(y + 1, x + w - 1, ACS_VLINE, h - 2);
	move(y + h - 1, x + w - 1);
	addch(ACS_LRCORNER);

	// draw title
	move(y, x + (w - title.length()) / 2);
	addstr(title.c_str());

	// draw contents
	uint16_t lines = 0, columns = 0;
	move(y + 1, x + 1);
	for (size_t i = 0; i<contents.length(); ++i) {
		switch (contents[i]) {
			case '\n': {
				++ lines;
				columns = 0;
				move(y + 1 + lines, x + 1);
				break;
			}
			default: {
				// check if the character will fit inside of the window borders
				if ((lines < h - 2) && (columns < w - 2)) {
					++ columns;
					addch(contents[i]);
				}
				break;
			}
		}
	}

	attroff(COLOR_PAIR(colour_pair));
}

bool UI::Window::ButtonPressed(string button) {
	return true;
}