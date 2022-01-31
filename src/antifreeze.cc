#include "_components.hh"
#include "uiwindow.hh"
#include "colourpairs.hh"
#include "iohandle.hh"

void antifreeze(uint16_t& freezetime, UI::Window& textbox, bool& run) {
	UI::Window freezewin;
	freezewin.x = 0;
	freezewin.y = 0;
	freezewin.colour_pair = COLOUR_PAIR_BAR;
	freezewin.title = "yedit has frozen";
	freezewin.contents = "yedit has frozen\nplease report this to MESYETI and tell him how\nto reproduce this error, press space to continue\nto yedit (your work has been saved)";
	while (run) {
		if (freezetime > 3000) {
			textbox.textboxInput         = "";
			textbox.textboxFinishedInput = true;
			while (getch() != ' ') {
				freezewin.w = COLS;
				freezewin.h = LINES;
				clear();
				freezewin.Render();
				refresh();
				usleep(1000000/60);
			}
		}
		usleep(25000);
	}
}