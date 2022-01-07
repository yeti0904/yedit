#pragma once
#include "_components.hh"
#include "alert.hh"

namespace Editor {
	void Render(string statusbar, vector <string>& fbuf, size_t scrollY, size_t curx, size_t cury);
	void SaveFile(string& fname, vector <string>& fbuf, UI::Alert& alert);
	void Backspace(vector <string>& fbuf, size_t& curx, size_t& cury);
	void Newline(vector <string>& fbuf, size_t& curx, size_t& cury);
};
