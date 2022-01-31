#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

class Properties {
	private:
		struct propertiesBuffer {
			std::string name;
			std::string value;
		};
		std::string unparsedBuffer;
		std::vector <propertiesBuffer> propsBuffer;
	public:
		Properties() {};
		void read(std::string buf) {
			std::vector <std::string> lines;
			std::string reading;
			std::string tmpName;
			std::string tmpValue;
			size_t      equLocation;
			int         equAmount;
			for (size_t i = 0; i<buf.length(); ++i) {
				if (buf[i] == 10) { // the new line character, \n
					lines.push_back(reading);
					reading = "";
				}
				else {
					reading += buf[i];
				}
			}
			// now lex
			propsBuffer = {}; // clear the properties buffer
			for (size_t i = 0; i<lines.size(); ++i) {
				if ((lines[i][0] != '#') && (lines[i].length() > 0)) { // ignore comments
					equAmount = 0;
					for (size_t i2 = 0; i2<lines[i].length(); ++i2) {
						if (lines[i][i2] == '=') {
							if (equAmount == 0) {
								equLocation = i2;
							}
							++ equAmount;
						}
					}
					if (equAmount == 0) {
						throw std::runtime_error((std::string)"Syntax error (line " + std::to_string(i) +"): value left undefined");
					}
					else if (equAmount > 1) {
						throw std::runtime_error((std::string)"Syntax error (line" + std::to_string(i) +"): too many instances of '='");
					}
					// begin lexing
					tmpName  = "";
					tmpValue = "";
					for (size_t i2 = 0; lines[i][i2] != '='; ++i2) {
						if (lines[i][i2] != ' ')
							tmpName += lines[i][i2];
					}
					for (size_t i2 = equLocation + 1; i2 < lines[i].length(); ++i2) {
						if ((lines[i][i2] != ' ') || ((lines[i][i2] == ' ') && tmpValue.length() != 0))
							tmpValue += lines[i][i2];
					}
					propsBuffer.push_back({
						tmpName,
						tmpValue
					});
				}
			}
		}
		void readFromFile(std::string fname) {
			std::ifstream fhnd(fname);
			std::string fstr = "";
			if (fhnd.is_open()) {
				std::string fline;
				while (getline(fhnd, fline)) {
	   				fstr += fline + "\n";
				}
				fhnd.close();
			}
			read(fstr);
		}
		bool propertyExists(std::string name) {
			for (size_t i = 0; i<propsBuffer.size(); ++i) {
				if (propsBuffer[i].name == name)
					return true;
			}
			return false;
		}
		std::string stringify() {
			std::string ret;
			for (size_t i = 0; i<propsBuffer.size(); ++i) {
				ret += propsBuffer[i].name + '=' + propsBuffer[i].value + '\n';
			}
			return ret;
		}
		void listProps(std::vector <std::string> &arr) {
			for (size_t i = 0; i<propsBuffer.size(); ++i) {
				arr.push_back(propsBuffer[i].name);
			}
		}
		void clear() {
			propsBuffer.clear();
		}
		std::string& operator[](std::string name) {
			bool   found = false;
			size_t i;
			for (i = 0; i<propsBuffer.size(); ++i) {
				if (propsBuffer[i].name == name) {
					found = true;
					break;
				}
			}
			if (!found)
				propsBuffer.push_back({
					name,
					""
				});
			// return the contents of the value
			return propsBuffer[i].value;
		}
};