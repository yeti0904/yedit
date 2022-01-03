#pragma once
#include "_components.hh"

namespace UI {
	class Window {
		public:
			Window() {};
			uint16_t        x, y, w, h;
			int             colour_pair;
			string          title;
			string          contents;
			vector <string> buttons;
			void Render();
			bool ButtonPressed(string button);
	};
};