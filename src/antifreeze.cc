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
	freezewin.contents = "yedit has frozen\nplease report this to MESYETI and tell him how\nto reproduce this error, press space to kill yedit";
	int16_t killTimer = 6000;

	while (run) {
		if (freezetime > 3000) {
			textbox.textboxInput         = "";
			textbox.textboxFinishedInput = true;
			while (getch() != ' ') {
				freezewin.w = COLS;
				freezewin.h = LINES;
				freezewin.Render();
				refresh();
				freezewin.contents = "yedit has frozen\n"
				"please report this to MESYETI and tell him how\n"
				"to reproduce this error\n"
				"press space to kill yedit or wait for the kill timer to end\n\n"
				"killing in " + to_string((int) (killTimer / 1000)) + " seconds";
				killTimer -= 1000/25;
				if (killTimer <= 0) {
					IOHandle::Exit();
					exit(1);
				}
				usleep(1000000/25);
			}
			IOHandle::Exit();
			exit(1);
		}
		freezetime += 1000/25;
		usleep(1000000/25);
	}
}