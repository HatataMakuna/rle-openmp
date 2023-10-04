#include "ppm.h"
#include <fstream>
#include <exception>
#include <sstream>
#include "consts.h"

using std::ifstream;
using std::ios;
using std::exception;
using std::stringstream;

PPM::PPM(const string& path) {
	// Open the file identified by the given path
	ifstream file{ path, ios::binary };

	// Read PPM header
	string header;
	std::getline(file, header);

	// Is binary PPM file ?
	if (header != "P6") throw exception{ "Invalid PPM file given" };

	// Read image dimensions
	string size;
	std::getline(file, size);
	stringstream{ size } >> width >> height;

	// Read max pixel component value
	string maxComponentStr;
	std::getline(file, maxComponentStr);
	unsigned int maxComponent;
	stringstream{ maxComponentStr } >> maxComponent;

	// Read pixels
	const auto pixelCount{ width * height * COMPONENT_COUNT };
	pixels.resize(pixelCount);
	file.read(reinterpret_cast<char*>(pixels.data()), pixelCount);
}
