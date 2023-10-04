#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

class PPM final {
	unsigned int width;
	unsigned int height;
	vector<unsigned char> pixels;

public:
	PPM(const string& path);

	constexpr unsigned int getWidth() const {
		return width;
	}

	constexpr unsigned int getHeight() const {
		return height;
	}

	constexpr vector<unsigned char>& getPixels() {
		return pixels;
	}
};
