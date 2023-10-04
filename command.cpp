#include "command.h"
#include "option.h"
#include "common.h"
#include <exception>
#include <filesystem>

using std::exception;
namespace fs = std::filesystem;

Command::Command(const char* argv[]) : option{ Option::NONE } {
	for (auto i{ 1 }; i != 3; ++i) {
		// Is this argument an option ?
		if (startsWith(argv[i], '-')) {
			// User has already specified an option ?
			if (option != Option::NONE) throw exception{ "Cannot specify multiple options" };

			if (string{ argv[i] } == "-e") { // User wants to encode PPM file ?
				option = Option::ENCODE;
			}
			else if (string{ argv[i] } == "-d") { // User wants to decode RLE file ?
				option = Option::DECODE;
			}
			else throw exception{ "Unrecognized option encountered" };
		}
		else {
			// User has already specified a source path ?
			if (srcPath != "") throw exception{ "Cannot specify multiple folder paths" };

			// This argument is a source path
			srcPath = argv[i];
		}
	}
}

string Command::getSrcPath() const {
	return srcPath;
}

void Command::exec() const {
	switch (option) {
	case Option::ENCODE:
		for (const auto& entry : fs::directory_iterator(srcPath)) {
			if (entry.path().string().ends_with(".ppm"))
				write(substr(entry.path().string(), '.') + ".rle", rleEncode(entry.path().string()));
		}

		break;
	case Option::DECODE:
		for (const auto& entry : fs::directory_iterator(srcPath)) {
			if (entry.path().string().ends_with(".rle"))
				write(substr(entry.path().string(), '.') + ".ppm", rleDecode(entry.path().string()));
		}

		break;
	default:
		throw exception{ "Option is required" };
	}
}
