#ifdef _WIN32
	#error yedit does not support windows
#endif

/*
	yedit: a free text editor
	Copyright (C) 2022  MESYETI

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
#include "alert.hh"
#include "settings.hh"
#include "constants.hh"

int main(int argc, char** argv) {
	vector <string> args;
	vector <string> fbuf        = {""};
	string          fname       = "Unnamed.txt";
	bool            noticeShown = true;
	bool            saved       = false;
	uint16_t        input;
	size_t          cury        = 0;
	size_t         	curx        = 0;
	size_t          scrollY     = 0;
	bool            run         = true;

	UI::Alert alert;

	Editor::Settings editorSettings;
	editorSettings.tabSize = 4;

	for (size_t i = 0; i<argc; ++i) {
		args.push_back(argv[i]);
	}

	if (args.size() >= 1) for (size_t i = 1; i<args.size(); ++i) {
		switch (args[i][0]) {
			case '-': {
				if ((args[i] == "-v") || (args[i] == "--version")) {
					puts(APP_NAME);
					return 0;
				}
				if ((args[i] == "-h") || (args[i] == "--help")) {
					puts(
						"Usage:\n"
						"yedit [filename] [options]\n\n"
						"Options:\n"
						"-v / --version : Print version\n"
						"-h / --help    : Print help"
					);
					return 0;
				}
				break;
			}
			default: {
				fname = args[i];
				Editor::OpenFile(fname, fbuf, alert);
				break;
			}
		}
	}

	IOHandle::Init();

	UI::Window notice;
	notice.x = 2;
	notice.y = 2;
	notice.w = COLS - 4;
	notice.h = LINES - 4;
	notice.colour_pair = COLOUR_PAIR_BAR;
	notice.title = "Notice";
	notice.contents = 
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

	UI::Window textbox;
	textbox.isTextbox    = true;
	textbox.x             = COLS / 2 - 15;
	textbox.y             = LINES / 2 - 3;
	textbox.w             = 30;
	textbox.h             = 5;
	textbox.contents      = "Press enter to finish";
	textbox.TextboxReset();
	textbox.textboxFinishedInput = true;

	while (run) {
		Editor::Render("fn: " + fname + " | lines: " + to_string(fbuf.size()), fbuf, scrollY, curx, cury, editorSettings);
		if (alert.time > 0) {
			alert.DoTime();
			alert.Render();
		}
		if (noticeShown)
			notice.Render();
		if (!textbox.textboxFinishedInput) {
			textbox.Render();
			textbox.x = COLS / 2 - 15;
			textbox.y  = LINES / 2 - 3;
		}
		if (textbox.textboxFinishedInput) {
			Editor::Input(fbuf, curx, cury, alert, notice, run, noticeShown, scrollY, fname, textbox);
		}
		else {
			run = textbox.TextboxInput();
			if (textbox.textboxFinishedInput) {
				if (textbox.title == "Save") {
					fname = textbox.textboxInput;
					Editor::SaveFile(fname, fbuf, alert);
				}
				else if (textbox.title == "Open") {
					fname = textbox.textboxInput;
					Editor::OpenFile(fname, fbuf, alert);
					curx = 0;
					cury = 0;
					scrollY = 0;
				}
				else if (textbox.title == "Find") {
					for (size_t i = 0; i < fbuf.size(); ++i) {
						if (fbuf[i].find(textbox.textboxInput) != string::npos) {
							cury = i;
							curx = fbuf[i].find(textbox.textboxInput);
							scrollY = cury;
							break;
						}
					}
				}
				else if (textbox.title == "Command") {
					if (textbox.textboxInput == "toggle render null") {
						editorSettings.debugNull = !editorSettings.debugNull;
						alert.time               = 3000;
						alert.text               = string("null render set to ") + (editorSettings.debugNull ? "on" : "off");
					}
					else {
						alert.time = 3000;
						alert.text = "Unknown command";
					}
				}
			}
		}
		refresh();
		usleep(1000000 / 60); // 60fps
	}
	IOHandle::Exit();

	return 0;
}
