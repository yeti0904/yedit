#pragma once
#include "_components.hh"

namespace UI {
	class Window {
		public:
			Window();
			uint16_t        x, y, w, h;
			int             colour_pair;
			string          title;
			string          contents;
			vector <string> buttons;

			// other stuff
			bool    isTextbox;
			uint8_t textboxColour;
			bool    isSelection;
			uint8_t selectionColour;
		
			void Render();
			bool ButtonPressed(string button);

			// textbox stuff
			string textboxInput;
			size_t textboxCurx;
			size_t textboxScroll;
			bool   textboxFinishedInput;
			void   TextboxReset();
			bool   TextboxInput();

			// selection stuff
			size_t          selectionSelected;
			vector <string> selectionButtons;
			bool            selectionFinishedInput;
			void            SelectionReset();
			bool            SelectionInput();
	};
};