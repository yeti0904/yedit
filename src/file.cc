#include "file.hh"

void File::Write(string fname, string content) {
	ofstream fhnd(fname);
	fhnd << content;
	fhnd.close();
}

string File::Read(string fname) {
	ifstream fhnd(fname);
	string   ret;
	string   line;
	while (getline(fhnd, line)) {
		ret += line + "\n";
	}
	fhnd.close();
	return ret;
}

void File::CreateDirectory(string name) {
	mkdir(name.c_str(), 0777);
}

void File::CreateFile(string fname) {
	ofstream fhnd(fname);
	fhnd.close();
}

bool File::DirExists(string path) {
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
		return false;
	else if (info.st_mode & S_IFDIR)
		return true;
	return false;
}