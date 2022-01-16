#pragma once
#include "_components.hh"

namespace File {
	void Write(string fname, string content);
	void CreateDirectory(string name);
	void CreateFile(string fname);
}