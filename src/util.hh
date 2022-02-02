#pragma once
#include "_components.hh"
#include "settings.hh"

namespace Util {
	int             StringToColour(string colour);
	void            UpdateSettings(Properties& props, Properties& theme, Editor::Settings& editorSettings);
	bool            IsNumber(string str);
	bool            StringEndsWith(string src, string suffix);
	vector <string> GetThemes();
}