#pragma once
#include "_components.hh"

namespace Editor {
	struct Settings {
		Settings() {
			tabSize     = 4;
			debugNull   = false;
			lineNumbers = false;
		}
		uint8_t tabSize;
		bool    debugNull;
		bool    lineNumbers;
	};
}