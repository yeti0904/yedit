#include "util.hh"
#include "file.hh"
#include "uiwindow.hh"
#include "iohandle.hh"
#include "editor.hh"

int Util::StringToColour(string colour) {
	if (colour == "black") {
		return COLOR_BLACK;
	}
	else if (colour == "red") {
		return COLOR_RED;
	}
	else if (colour == "yellow") {
		return COLOR_YELLOW;
	}
	else if (colour == "green") {
		return COLOR_GREEN;
	}
	else if (colour == "blue") {
		return COLOR_BLUE;
	}
	else if (colour == "magenta") {
		return COLOR_MAGENTA;
	}
	else if (colour == "cyan") {
		return COLOR_CYAN;
	}
	else if (colour == "white") {
		return COLOR_WHITE;
	}
	else if (colour == "grey") {
		return COLOR_GREY;
	}
	else if (colour == "lightred") {
		return COLOR_LIGHTRED;
	}
	else if (colour == "lightgreen") {
		return COLOR_LIGHTGREEN;
	}
	else if (colour == "lightyellow") {
		return COLOR_LIGHTYELLOW;
	}
	else if (colour == "lightblue") {
		return COLOR_LIGHTBLUE;
	}
	else if (colour == "lightmagenta") {
		return COLOR_LIGHTMAGENTA;
	}
	else if (colour == "lightcyan") {
		return COLOR_LIGHTCYAN;
	}
	else if (colour == "lightwhite") {
		return COLOR_LIGHTWHITE;
	}
	return COLOR_BLACK;
}

void Util::UpdateSettings(Properties& props, Properties& theme, Editor::Settings& editorSettings) {
	File::Write(string(getenv("HOME")) + "/.config/yedit8/settings.properties",
		"# yedit settings file\n\n"
		"# tab size (default: 4)\n"
		"tabsize = " + props["tabsize"] + "\n" /* error here */
		"linenumbers = " + props["linenumbers"] + "\n\n"
		"# theme (default: retro)\n"
		"# themes are located in the themes folder\n"
		"theme = " + props["theme"] +"\n"
	);

	// reset yedit settings
	theme.clear();

	// load new settings
	editorSettings.tabSize     = stoi(props["tabsize"]);
	editorSettings.lineNumbers = (props["linenumbers"] == "true");
	theme.read(File::Read(string(getenv("HOME")) + "/.config/yedit8/themes/" + props["theme"] + ".properties"));

	// restart yedit
	IOHandle::Exit();
	IOHandle::Init(theme);
}

bool Util::IsNumber(string str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

bool Util::StringEndsWith(string src, string suffix) {
	// check if src ends with suffix
	if (src.length() < suffix.length()) {
		return false;
	}
	for (size_t i = 0; i < suffix.length(); i++) {
		if (src[src.length() - suffix.length() + i] != suffix[i]) {
			return false;
		}
	}
	return true;
}

vector <string> Util::GetThemes() {
	vector <string> themes;
	DIR*            dhnd;
	struct dirent*  dir;
	string          themedir = string(getenv("HOME")) + "/.config/yedit8/themes/";

	dhnd = opendir(themedir.c_str());

	if (dhnd) {
		while ((dir = readdir(dhnd)) != NULL) {
			if (StringEndsWith(dir->d_name, ".properties")) {
				themes.push_back(string(dir->d_name).substr(0, string(dir->d_name).find('.')));
			}
		}
	}

	return themes;
}