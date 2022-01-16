#pragma once
#include "_components.hh"
#include "alert.hh"
#include "uiwindow.hh"
#include "settings.hh"

namespace Editor {
	void Render(string statusbar, vector <string>& fbuf, size_t scrollY, size_t curx, size_t cury, Editor::Settings settings);
	void SaveFile(string& fname, vector <string>& fbuf, UI::Alert& alert);
	void OpenFile(string& fname, vector <string>& fbuf, UI::Alert& alert);
	void Backspace(vector <string>& fbuf, size_t& curx, size_t& cury);
	void Newline(vector <string>& fbuf, size_t& curx, size_t& cury);
	void Input(vector <string>& fbuf, size_t& curx, size_t& cury, UI::Alert& alert, UI::Window& notice,
	bool& run, bool& noticeShown, size_t& scrollY, string& fname, UI::Window& textbox);
};
