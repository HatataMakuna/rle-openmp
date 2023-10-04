#include <cstdlib>
#include <iostream>
#include <string>
#include "command.h"
#include <exception>
#include <chrono>

using std::string;
using std::exception;
using std::chrono::high_resolution_clock;

void showUsage(const string& name) {
	std::cerr << "Usage: " << name << " (-e|-d) FOLDER_PATH\n"
		<< "\tOptions:\n"
		<< "\t-e\tConverts the PPM files in the folder identified by FOLDER_PATH into run-length encoded (.rle) files\n"
		<< "\t-d\tConverts the run-length encoded (.rle) files in the folder identified by FOLDER_PATH back to the original (.ppm) files\n"
		<< "\n"
		<< "\tFOLDER_PATH:\n"
		<< "\tThe path to the folder that contains the image files to be processed. Image files Can be either PPM file or run-length encoded (.rle) file\n";
}

// Example command line input: "x64/Release/rle.exe" -e images
int main(const int argc, const char* argv[]) try {
	// User did not supplied required arguments ?
	if (argc <= 2) throw exception{ "Required arguments are not fully supplied" };

	// Process
	const auto start{ high_resolution_clock::now() };
	Command{ argv }.exec();
	const auto end{ high_resolution_clock::now() };

	// Print the benchmark result
	std::cout << "Serial time taken: "
		<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9
		<< "s\n";

	// So that the user can view the console output before exiting the program
	system("pause");

	return 0;
}
catch (const exception& ex) {
	std::cerr << "Error: " << ex.what() << "\n\n";
	showUsage(argv[0]);
	return 1;
}
