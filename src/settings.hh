#pragma once
#include "_components.hh"

namespace Editor {
	struct Settings {
		Settings() {
			tabSize   = 4;
			debugNull = false;
		}
		uint8_t tabSize;
		bool    debugNull;
	};
}