#pragma once
#include "_components.hh"

namespace Tabs {
	struct Structure {
		string fname;
		size_t curx, cury, scrollY;
	};
	void Load(Structure& tab, string& fname, size_t& curx, size_t& cury, size_t& scrollY);
}