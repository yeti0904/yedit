#include "_components.hh"
#include "editor.hh"
#include "constants.hh"
#include "colourpairs.hh"
#include "iohandle.hh"
#include "terminal.hh"

const string currentTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);
	return buf;
}

void Editor::Render(string statusbar, vector <string>& fbuf, size_t scrollY, size_t curx, size_t cury, Editor::Settings settings) {
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

	/*

	Here lies yedit memory usage
	3rd January 2022 - 14th January 2022

	attron(COLOR_PAIR(COLOUR_PAIR_MEM));
	string mem;
	// get memory usage percentage
	mem = to_string((int)((((double) info.totalram - (double) info.freeram) / info.totalram) * 100)) + "%";
	move(0, COLS - mem.length() - currentTime().length());
	addstr(mem.c_str());
	attroff(COLOR_PAIR(COLOUR_PAIR_MEM));
	*/

	// render editor
	attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));
	for (int i = 1; i < LINES - 1; ++i) {
		mvhline(i, 0, ' ', COLS);
	}

	// editor contents variables
	size_t lines = fbuf.size();

	// render editor contents
	for (size_t i = scrollY; i < fbuf.size(); ++i) {
		//if (i + scrollY < fbuf.size()) {
			if (settings.lineNumbers) {
				move(i - scrollY + 1, 0);
				addstr(to_string(i + 1).c_str());
				move(i - scrollY + 1, to_string(lines + 1).length());
				addch(ACS_VLINE);
				move(i - scrollY + 1, to_string(lines + 1).length() + 2);
			}
			else
				move(i - scrollY + 1, 0);
			// addstr(fbuf[i + scrollY].c_str());
			for (size_t j = 0; j <= fbuf[i].length(); ++j) {
				if ((cury == i) && (curx == j))
					attron(A_REVERSE);
				switch (fbuf[i][j]) {
					case '\t': {
						if ((cury == i) && (curx == j)) {
							addch(' ');
							attroff(COLOR_PAIR(COLOUR_PAIR_CURSOR));
							attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));
							for (size_t k = 0; k < settings.tabSize - 1; ++k) {
								addch(' ');
							}
						}
						else
							for (size_t k = 0; k < settings.tabSize; ++k) {
								addch(' ');
							}
						break;
					}
					default: {
						if ((fbuf[i][j] == '\0') && !settings.debugNull)
							addch(' ');
						else if (fbuf[i][j] != '\r')
							addch(fbuf[i][j]);
						break;
					}
				}
				if ((cury == i) && (curx == j)) {
					attroff(A_REVERSE);
				}
			}
		//}
	}

	attroff(COLOR_PAIR(COLOUR_PAIR_EDITOR));

	// render cursor
	/*

	Here lies old yedit cursor code
	(idk) - 15th January 2022

	attron(COLOR_PAIR(COLOUR_PAIR_CURSOR));
	move(cury + 1 + scrollY, curx);
	if (curx >= fbuf[cury].length())
		addch(' ');
	else if (fbuf[cury][curx] == '\t') {
		addch(' ');
		attroff(COLOR_PAIR(COLOUR_PAIR_CURSOR));
		attron(COLOR_PAIR(COLOUR_PAIR_EDITOR));
		for (uint8_t i = 0; i < tabSize - 1; ++i) {
			addch(' ');
		}
		attroff(COLOR_PAIR(COLOUR_PAIR_EDITOR));
	}
	else
		addch(fbuf[cury][curx]);
	attroff(COLOR_PAIR(COLOUR_PAIR_CURSOR));
	*/

	// render status bar
	attron(COLOR_PAIR(COLOUR_PAIR_BAR));
	mvhline(LINES - 1, 0, ' ', COLS);
	move(LINES - 1, 0);
	addstr(statusbar.c_str());
	attroff(COLOR_PAIR(COLOUR_PAIR_BAR));
}

void Editor::SaveFile(string& fname, vector <string>& fbuf, UI::Alert& alert) {
	ofstream fhnd(fname);
	if (!fhnd.is_open()) {
		alert.text = "Failed to save";
		alert.time = 3000;
		return;
	}
	for (size_t i = 0; i<fbuf.size(); ++i) {
		fhnd << fbuf[i] << "\n";
	}
	fhnd.close();
	alert.text = "Saved to " + fname;
	alert.time = 3000;
}

void Editor::OpenFile(string& fname, vector <string>& fbuf, UI::Alert& alert) {
	ifstream fhnd(fname);
	if (!fhnd.is_open()) {
		alert.text = "Failed to open";
		alert.time = 3000;
		return;
	}
	fbuf.clear();
	string line;
	while (getline(fhnd, line)) {
		fbuf.push_back(line);
	}
	fhnd.close();
	alert.text = "Opened " + fname;
	alert.time = 3000;
}

void Editor::Backspace(vector <string>& fbuf, size_t& curx, size_t& cury) {
	if (curx > 0) {
		fbuf[cury].erase(curx - 1, 1);
		-- curx;
	}
	else {
		if (cury > 0) {
			fbuf[cury - 1] += fbuf[cury];
			fbuf.erase(fbuf.begin() + cury);
			-- cury;
			curx = fbuf[cury].size();
		}
	}
}

void Editor::Newline(vector <string>& fbuf, size_t& curx, size_t& cury, size_t& scrollY) {
	// insert if not at end of line
	if (curx < fbuf[cury].size()) {
		fbuf.insert(fbuf.begin() + cury + 1, fbuf[cury].substr(curx));
		fbuf[cury].erase(curx);
	}
	else {
		// insert new line
		fbuf.insert(fbuf.begin() + cury + 1, "");
	}
	curx = 0;
	++ cury;
	// scroll down if needed
	if (cury >= (LINES - 2)) {
		++ scrollY;
	}
}

void Editor::Input(
	vector <string>& fbuf, size_t& curx, size_t& cury, UI::Alert& alert, UI::Window& notice, 
	bool& run, bool& noticeShown, size_t& scrollY, string& fname, UI::Window& textbox,
	Properties& theme, string& clipboard
) {
	uint16_t input = getch();
	switch (input) {
		case KEY_RESIZE: {
			notice.w = COLS - 4;
			notice.h = LINES - 4;
			break;
		}
		case 127:
		case KEY_BACKSPACE: {
			Editor::Backspace(fbuf, curx, cury);
			break;
		}
		case KEY_LEFT: {
			if (curx > 0)
				--curx;
			else if (cury > 0) {
				--cury;
				curx = fbuf[cury].size();
			}
			break;
		}
		case KEY_RIGHT: {
			if (curx < fbuf[cury].size())
				++curx;
			else if (cury < fbuf.size() - 1) {
				++cury;
				curx = 0;
			}
			break;
		}
		case KEY_UP: {
			if (cury > 0) {
				--cury;
				if (curx > fbuf[cury].size())
					curx = fbuf[cury].size();
				/*if (scrollY > 0)
					-- scrollY;*/
				// scroll up if the cursor is above the viewable area
				if (cury < scrollY)
					scrollY = cury;
			}
			break;
		}
		case KEY_DOWN: {
			if (cury < fbuf.size() - 1) {
				++cury;
				if (curx > fbuf[cury].size())
					curx = fbuf[cury].size();
				// scroll down if the cursor is beyond the viewable area
				if (cury > scrollY + LINES - 3)
					scrollY = cury - LINES + 4;
			}
			break;
		}
		case KEY_NPAGE: {
			if (cury < fbuf.size() - 4) {
				scrollY += 4;
				cury    += 4;
			}
			break;
		}
		case KEY_PPAGE: {
			if (cury > 4) {
				scrollY -= 4;
				cury    -= 4;
			}
			break;
		}
		case ctrl('s'): {
			// save
			if (fname == "Unnamed.txt") {
				textbox.TextboxReset();
				textbox.contents             = "Enter file name:";
				textbox.title                = "Save";
				textbox.textboxFinishedInput = false;
			}
			else Editor::SaveFile(fname, fbuf, alert);
			break;
		}
		case ctrl('w'): {
			// Save As
			textbox.TextboxReset();
			textbox.contents             = "Enter file name:";
			textbox.title                = "Save";
			textbox.textboxFinishedInput = false;
		}
		case ctrl('o'): {
			// open
			textbox.TextboxReset();
			textbox.contents             = "Enter file name:";
			textbox.title                = "Open";
			textbox.textboxFinishedInput = false;
			break;
		}
		case ctrl('q'): {
			run = false;
			break;
		}
		case ctrl('t'): {
			Terminal::Run(theme);
			break;
		}
		case ctrl('f'): {
			textbox.TextboxReset();
			textbox.contents             = "String to find:";
			textbox.title                = "Find";
			textbox.textboxFinishedInput = false;
			break;
		}
		case ctrl('e'): {
			textbox.TextboxReset();
			textbox.contents             = "";
			textbox.title                = "Command";
			textbox.textboxFinishedInput = false;
			break;
		}
		case ctrl('k'): {
			// cut line
			clipboard = fbuf[cury];
			fbuf[cury] = "";
			alert.text = "Cut line";
			alert.time = 3000;
			break;
		}
		case ctrl('l'): {
			// copy line
			clipboard = fbuf[cury];
			alert.text = "Copied line";
			alert.time = 3000;
			break;
		}
		case ctrl('u'): {
			// paste
			fbuf[cury].insert(curx, clipboard);
			alert.text = "Pasted";
			alert.time = 3000;
			break;
		}
		case ctrl('n'): {
			fbuf = {""};
			cury = 0;
			curx = 0;
			scrollY = 0;
			alert.text = "New file";
			alert.time = 3000;
			break;
		}
		case '\n': {
			Editor::Newline(fbuf, curx, cury, scrollY);
			break;
		}
		case ' ': {
			if (noticeShown) {
				noticeShown = false;
				break;
			}
		}
		default: {
			if (((input >= ' ') && (input <= '~')) || (input == '\t')) {
				++ curx;
				if (curx >= fbuf[cury].size()) fbuf[cury] += input;
				else fbuf[cury].insert(curx - 1, 1, input);
			}
			break;
		}
	}
}