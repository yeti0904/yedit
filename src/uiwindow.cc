#include "uiwindow.hh"
#include "colourpairs.hh"

UI::Window::Window() {
	colour_pair = COLOUR_PAIR_BAR;
	title = "Untitled window";
	textboxColour = COLOUR_PAIR_EDITOR;
	isTextbox = false;
}

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

	if (isTextbox) {
		// render textbox on the bottom line of the window
		attron(COLOR_PAIR(textboxColour));
		move(y + h - 2, x + 1);
		for (size_t i = 0; i <= textboxInput.length(); ++i) {
			if (i == textboxCurx)
				attron(A_REVERSE);

			if (textboxInput[i] != '\0') addch(textboxInput[i]);
			else addch(' ');

			if (i == textboxCurx)
				attroff(A_REVERSE);
		}
		if (w -3 > textboxInput.length())
			for (size_t i = 0; i<w - 3 - textboxInput.length(); ++i) {
				addch(' ');
			}
		attroff(COLOR_PAIR(textboxColour));
	}

	else if (isSelection) {
		attron(COLOR_PAIR(selectionColour));
		for (size_t i = selectionScrollY; (i - selectionScrollY < selectionButtons.size()) && (i - selectionScrollY < h - 2); ++i) {
			move(y + 1 + (i - selectionScrollY), x + 1);
			if (selectionSelected == i) {
				addch('>');
				attron(A_REVERSE);
			}
			mvhline(y + 1 + (i - selectionScrollY), x + 2, ' ', w - 3);
			move(y + 1 + (i - selectionScrollY), x + 2);
			addstr(selectionButtons[i].c_str());
			if (selectionSelected == i) attroff(A_REVERSE);
		}
		attroff(COLOR_PAIR(selectionColour));
		attron(COLOR_PAIR(colour_pair));
		if (selectionScrollY != 0) {
			mvaddch(y, x + w - 1, ACS_UARROW);
		}
		if (selectionScrollY + h - 2 < selectionButtons.size()) {
			mvaddch(y + h - 1, x + w - 1, ACS_DARROW);
		}
	}
}

bool UI::Window::ButtonPressed(string button) {
	return true;
}

void UI::Window::TextboxReset() {
	textboxInput         = "";
	textboxCurx          = 0;
	textboxFinishedInput = false;
}

bool UI::Window::TextboxInput() {
	uint16_t input = getch();
	switch (input) {
		case 127:
		case KEY_BACKSPACE: {
			if (textboxInput.length() > 0) {
				textboxInput.erase(textboxCurx - 1, 1);
				-- textboxCurx;
			}
			break;
		}
		case KEY_RIGHT: {
			if (textboxCurx < textboxInput.length()) {
				++ textboxCurx;
			}
			break;
		}
		case KEY_LEFT: {
			if (textboxCurx > 0) {
				-- textboxCurx;
			}
			break;
		}
		case ctrl('q'): {
			return false;
		}
		case '\n': {
			textboxFinishedInput = true;
			break;
		}
		default: {
			if (((input >= ' ') && (input <= '~')) || (input == '\t')) {
				textboxInput.insert(textboxCurx, 1, input);
				++ textboxCurx;
			}
			break;
		}
	}
	return true;
}

void UI::Window::SelectionReset() {
	selectionButtons.clear();
	selectionSelected       = 0;
	selectionFinishedInput = false;
	selectionScrollY       = 0;
}

bool UI::Window::SelectionInput() {
	uint16_t input = getch();
	switch (input) {
		case KEY_UP: {
			if (selectionSelected > 0) {
				-- selectionSelected;
				if (selectionSelected < selectionScrollY) {
					-- selectionScrollY;
				}
			}
			break;
		}
		case KEY_DOWN: {
			if (selectionSelected < selectionButtons.size() - 1) {
				++ selectionSelected;
				if (selectionSelected > selectionScrollY + h - 3) {
					++ selectionScrollY;
				}
			}
			break;
		}
		case ctrl('q'): {
			return false;
		}
		case ' ':
		case '\n': {
			selectionFinishedInput = true;
			break;
		}
		default: {
			break;
		}
	}
	return true;
}