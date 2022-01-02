#ifdef _WIN32
	#error yedit does not support windows
#endif

/*
	yedit: a free text editor
	Copyright (C) 2021  MESYETI

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "_components.hh"
#include "iohandle.hh"
#include "editor.hh"
#include "uiwindow.hh"
#include "colourpairs.hh"

int main(int argc, char** argv) {
	vector <string> args;
	vector <string> fbuf       = {""};
	string          fname      = "Unnamed.txt";
	bool            alertShown = true;
	uint16_t        input;
	size_t          cury       = 0;
	size_t         	curx       = 0;
	size_t          scrollY    = 0;
	bool            run        = true;

	for (size_t i = 0; i<argc; ++i) {
		args.push_back(argv[i]);
	}

	IOHandle::Init();

	UI::Window alert;
	alert.x = 2;
	alert.y = 2;
	alert.w = COLS - 4;
	alert.h = LINES - 4;
	alert.colour_pair = COLOUR_PAIR_BAR;
	alert.title = "Notice";
	alert.contents = 
	"(Press space to close this notice)\n"
	"This program is free software: you can redistribute it and/or modify\n"
    "it under the terms of the GNU General Public License as published by\n"
    "the Free Software Foundation, either version 3 of the License, or\n"
    "(at your option) any later version.\n"
	"\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU General Public License for more details.\n"
	"\n"
    "You should have received a copy of the GNU General Public License\n"
    "along with this program.  If not, see <https://www.gnu.org/licenses/>.\n";

	while (run) {
		Editor::Render("fn: " + fname, fbuf, scrollY, curx, cury);
		if (alertShown)
			alert.Render();
		input = getch();
		switch (input) {
			case KEY_RESIZE: {
				alert.w = COLS - 4;
				alert.h = LINES - 4;
				break;
			}
			case ctrl('q'): {
				run = false;
				break;
			}
			case '\n': {
				++ cury;
				if (cury >= fbuf.size()) fbuf.push_back("");
				break;
			}
			case ' ': {
				if (alertShown) {
					alertShown = false;
					break;
				}
			}
			default: {
				if (((input >= ' ') && (input <= '~')) || (input == '\t')) {
					++ curx;
					if (curx >= fbuf[cury].size()) fbuf[cury] += input;
					else fbuf[cury].insert(curx, 1, input);
				}
				break;
			}
		}
		usleep(1000000 / 60); // 60fps
	}
	IOHandle::Exit();

	return 0;
}
