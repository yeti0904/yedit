#include "file.hh"

void File::Write(string fname, string content) {
	ofstream fhnd(fname);
	fhnd << content;
	fhnd.close();
}

void File::CreateDirectory(string name) {
	mkdir(name.c_str(), 0777);
}

void File::CreateFile(string fname) {
	ofstream fhnd(fname);
	fhnd.close();
}