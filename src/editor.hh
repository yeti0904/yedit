#pragma once
#include "_components.hh"
#include "alert.hh"
#include "uiwindow.hh"
#include "settings.hh"
#include "vec2.hh"

namespace Editor {
	void Render(string statusbar, vector <string>& fbuf, size_t scrollY, size_t curx, size_t cury, Editor::Settings settings, size_t& scrollX);
	void SaveFile(string& fname, vector <string>& fbuf, UI::Alert& alert);
	void OpenFile(string& fname, vector <string>& fbuf, UI::Alert& alert);
	void Backspace(vector <string>& fbuf, size_t& curx, size_t& cury);
	void Newline(vector <string>& fbuf, size_t& curx, size_t& cury, size_t& scrollY);
	void Input(vector <string>& fbuf, size_t& curx, size_t& cury, UI::Alert& alert, UI::Window& notice,
	bool& run, bool& noticeShown, size_t& scrollY, string& fname, UI::Window& textbox,
	Properties& theme, string& clipboard, Properties& settings, Editor::Settings& editorSettings, UI::Window& selection,
	size_t& scrollX);
};
