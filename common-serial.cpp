#include "common.h"
#include <fstream>
#include <exception>
#include <sstream>
#include "consts.h"
#include "ppm.h"
#include <iostream>
#include "rle.h"

using std::ifstream;
using std::ofstream;
using std::ios;
using std::exception;
using std::stringstream;

bool startsWith(const string& from, const char target) {
	return from.rfind(target, 0) == 0;
}

vector<unsigned char> rleEncode(const string& path) {
	PPM ppm{ path };
	vector<unsigned char> result;
	result.reserve(ppm.getPixels().size() + ppm.getWidth() * ppm.getHeight() + 4);

	// Write size information so that later can decode
	unsigned char sizeBits[4];
	toUChars(sizeBits, ppm.getWidth());
	toUChars(sizeBits + 2, ppm.getHeight());
	std::copy(sizeBits, sizeBits + sizeof(sizeBits), std::back_inserter(result));

	// Assume we have encountered a pixel that is different from the first pixel
	unsigned char prevPixel[]{ 255 - ppm.getPixels()[0], 255 - ppm.getPixels()[1], 255 - ppm.getPixels()[2] };
	unsigned char occurenceCount{ 0 };

	// TODO: Speed up this function by multiprocessing
	for (auto i{ 0 }; i < ppm.getPixels().size(); i += COMPONENT_COUNT) {
		// Have we encountered this pixel previously ?
		if (
			ppm.getPixels()[i] == prevPixel[0] && ppm.getPixels()[i + 1] == prevPixel[1] &&
			ppm.getPixels()[i + 2] == prevPixel[2] && occurenceCount != UCHAR_MAX
			) {
			++occurenceCount;
		}
		else {
			// Ignore the assumed pixel at the start
			if (occurenceCount != 0) {
				// Write the previous pixel occurrence count
				result.push_back(occurenceCount);
			}

			// Write this pixel
			result.push_back(ppm.getPixels()[i]); // Red
			result.push_back(ppm.getPixels()[i + 1]); // Green
			result.push_back(ppm.getPixels()[i + 2]); // Blue

			// Record the inserted pixel for checking later
			prevPixel[0] = ppm.getPixels()[i];
			prevPixel[1] = ppm.getPixels()[i + 1];
			prevPixel[2] = ppm.getPixels()[i + 2];

			// Reset occurrence count for this pixel
			occurenceCount = 1;
		}
	}

	// Write the last pixel occurrence count
	result.push_back(occurenceCount);

	return result;
}

vector<unsigned char> rleDecode(const string& path) {
	RLE rle{ path };
	vector<unsigned char> result;
	result.reserve(rle.getWidth() * rle.getHeight() * COMPONENT_COUNT + 19); // 19 represents the size of the PPM header

	// Write PPM header
	pushStr(result, "P6\n");
	pushStr(result, std::to_string(rle.getWidth()));
	pushStr(result, " ");
	pushStr(result, std::to_string(rle.getHeight()));
	pushStr(result, "\n");
	pushStr(result, "255\n");

	// TODO: Speed up this function by multiprocessing
	for (auto i{ 0 }; i < rle.getPixels().size(); i += COMPONENT_COUNT + 1) {
		for (auto j{ 0 }; j < rle.getPixels()[i + 3]; ++j) {
			result.push_back(rle.getPixels()[i]);
			result.push_back(rle.getPixels()[i + 1]);
			result.push_back(rle.getPixels()[i + 2]);
		}
	}

	return result;
}

void toUChars(unsigned char* const to, const uint16_t from) {
	to[0] = from >> 8 & 0xFF;
	to[1] = from & 0xFF;
}

uint16_t toU16(const unsigned char* const from) {
	return (from[0] << 8) + from[1];
}

void write(const string& path, const vector<unsigned char>& data) {
	ofstream file{ path, ios::binary };
	file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

string substr(const string& from, const char end) {
	const auto index{ from.rfind(end) };
	return from.substr(0, index != string::npos ? index : SIZE_MAX);
}

void pushStr(vector<unsigned char>& to, const string& value) {
	const auto bits{ value.c_str() };
	std::copy(bits, bits + value.length(), std::back_inserter(to));
}