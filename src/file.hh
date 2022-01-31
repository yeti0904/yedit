#pragma once
#include "_components.hh"

namespace File {
	void   Write(string fname, string content);
	string Read(string fname);
	void   CreateDirectory(string name);
	void   CreateFile(string fname);
	bool   DirExists(string path);
}