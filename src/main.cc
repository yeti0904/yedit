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
#include "file.hh"
#include "antifreeze.hh"
#include "vec2.hh"
#include "util.hh"

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
	uint16_t        freezetime  = 0;
	string          clipboard   = "";

	if (!File::DirExists(string(getenv("HOME")) + "/.config/yedit8")) {
		File::CreateDirectory(string(getenv("HOME")) + "/.config/yedit8");
		File::Write(string(getenv("HOME")) + "/.config/yedit8/settings.properties",
			"# yedit settings file\n\n"
			"# tab size (default: 4)\n"
			"tabsize = 4\n"
			"linenumbers = false\n\n"
			"# theme (default: retro)\n"
			"# themes are located in the themes folder\n"
			"theme = retro\n"
		);
		File::CreateDirectory(string(getenv("HOME")) + "/.config/yedit8/themes");
		File::Write(string(getenv("HOME")) + "/.config/yedit8/themes/retro.properties",
			"editorBG = blue\n"
			"editorFG = white\n"
			"barBG    = white\n"
			"barFG    = black\n"
			"timeBG   = green\n"
			"timeFG   = black\n"
		);
		File::Write(string(getenv("HOME")) + "/.config/yedit8/themes/bubblegum.properties",
			"editorBG = lightmagenta\n"
			"editorFG = white\n"
			"barBG    = magenta\n"
			"barFG    = white\n"
			"timeBG   = magenta\n"
			"timeFG   = white\n"
		);
		File::Write(string(getenv("HOME")) + "/.config/yedit8/themes/monochrome.properties",
			"editorBG = black\n"
			"editorFG = white\n"
			"barBG    = white\n"
			"barFG    = black\n"
			"timeBG   = white\n"
			"timeFG   = black\n"
		);
		File::Write(string(getenv("HOME")) + "/.config/yedit8/themes/dark.properties",
			"editorBG = black\n"
			"editorFG = white\n"
			"barBG    = cyan\n"
			"barFG    = black\n"
			"timeBG   = cyan\n"
			"timeFG   = black\n"
		);
		File::Write(string(getenv("HOME")) + "/.config/yedit8/themes/light.properties",
			"editorBG = white\n"
			"editorFG = black\n"
			"barBG    = cyan\n"
			"barFG    = black\n"
			"timeBG   = cyan\n"
			"timeFG   = black\n"
		);
		File::Write(string(getenv("HOME")) + "/.config/yedit8/themes/mono-light.properties",
			"editorBG = white\n"
			"editorFG = black\n"
			"barBG    = black\n"
			"barFG    = white\n"
			"timeBG   = black\n"
			"timeFG   = white\n"
		);
		File::Write(string(getenv("HOME")) + "/.config/yedit8/themes/16c-emerald.properties",
			"editorBG = green\n"
			"editorFG = white\n"
			"barBG    = lightgreen\n"
			"barFG    = lightwhite\n"
			"timeBG   = lightgreen\n"
			"timeFG   = lightwhite\n"
		);
		File::Write(string(getenv("HOME")) + "/.config/yedit8/themes/16c-newretro.properties",
			"editorBG = lightblue\n"
			"editorFG = white\n"
			"barBG    = white\n"
			"barFG    = black\n"
			"timeBG   = lightgreen\n"
			"timeFG   = black\n"
		);
	}

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
				else if ((args[i] == "-h") || (args[i] == "--help")) {
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

	Properties props;
	props.read(File::Read(string(getenv("HOME")) + "/.config/yedit8/settings.properties"));
	editorSettings.tabSize     = stoi(props["tabsize"]);
	editorSettings.lineNumbers = (props["linenumbers"] == "true");

	Properties theme;
	theme.read(File::Read(string(getenv("HOME")) + "/.config/yedit8/themes/" + props["theme"] + ".properties"));

	IOHandle::Init(theme);

	vec2 mark    = {0, 0};
	bool markSet = false;

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

	UI::Window selection;
	selection.isSelection = true;
	selection.x = COLS / 2 - 15;
	selection.y = LINES / 2 - 5;
	selection.w = 30;
	selection.h = 10;
	selection.selectionFinishedInput = true;
	

	thread antifreezeThread(antifreeze, ref(freezetime), ref(textbox), ref(run));

	while (run) {
		Editor::Render(
			"fn: " + fname + " | lines: " + to_string(fbuf.size()) + " | (" + to_string(curx) + ":" + to_string(cury) + ")"
		, fbuf, scrollY, curx, cury, editorSettings);
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
		if (!selection.selectionFinishedInput) {
			selection.Render();
			selection.x = COLS / 2 - 15;
			selection.y = LINES / 2 - 5;
		}
		freezetime = 0;
		if (textbox.textboxFinishedInput && selection.selectionFinishedInput) {
			Editor::Input(fbuf, curx, cury, alert, notice, run, noticeShown, scrollY, fname, textbox, theme,
				clipboard, props, editorSettings, selection);
		}
		else if (!textbox.textboxFinishedInput) {
			run = textbox.TextboxInput();
			if (textbox.textboxFinishedInput) {
				if (textbox.title == "Save") {
					fname = textbox.textboxInput;
					Editor::SaveFile(fname, fbuf, alert);
				}
				else if (textbox.title == "Open") {
					fname = textbox.textboxInput;
					if (fname == "$yeditconfig") {
						fname = string(getenv("HOME")) + "/.config/yedit8/settings.properties";
					}
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
				else if (textbox.title == "Change tab size") {
					// check if integer
					if (!Util::IsNumber(textbox.textboxInput)) {
						alert.text = "Tab size must be an integer";
						alert.time = 3000;
					}
					else {
						// set new settings
						props["tabsize"] = textbox.textboxInput;

						// update
						Util::UpdateSettings(props, theme, editorSettings);

						// show alert
						alert.text = "Set tab size to " + props["tabsize"];
						alert.time = 3000;
					}
				}
			}
		}
		else if (!selection.selectionFinishedInput) {
			run = selection.SelectionInput();
			if (selection.selectionFinishedInput) {
				if (selection.title == "Settings") {
					if (selection.selectionButtons[selection.selectionSelected] == "Change theme") {
						selection.SelectionReset();
						selection.title = "Change theme";
						selection.selectionButtons = Util::GetThemes();
					}
					else if (selection.selectionButtons[selection.selectionSelected] == "Change tab size") {
						textbox.TextboxReset();
						textbox.title = "Change tab size";
						textbox.contents = "Enter tab size as an integer";
					}
					else if (selection.selectionButtons[selection.selectionSelected] == "Toggle line numbers") {
						// toggle line numbers
						props["linenumbers"] = (props["linenumbers"] == "true")? "false" : "true";

						// update
						Util::UpdateSettings(props, theme, editorSettings);

						// show alert
						alert.text = "Line numbers set to " + string((props["linenumbers"] == "true")? "on" : "off");
						alert.time = 3000;
					}
					else if (selection.selectionButtons[selection.selectionSelected] == "Close settings") {}
				}
				else if (selection.title == "Change theme") {
					// change theme property
					props["theme"] = selection.selectionButtons[selection.selectionSelected];

					// update
					Util::UpdateSettings(props, theme, editorSettings);

					// set alert
					alert.text = "Set theme to " + props["theme"];
					alert.time = 3000;
				}
			}
		}
		refresh();
		usleep(1000000 / 60); // 60fps
	}
	IOHandle::Exit();
	antifreezeThread.join();

	return 0;
}
