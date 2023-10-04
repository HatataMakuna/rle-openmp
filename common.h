#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

bool startsWith(const string& from, const char target);
vector<unsigned char> rleEncode(const string& path);
vector<unsigned char> rleDecode(const string& path);
void toUChars(unsigned char* const to, const uint16_t from);
uint16_t toU16(const unsigned char* const from);
void write(const string& path, const vector<unsigned char>& data);
string substr(const string& from, const char end);
void pushStr(vector<unsigned char>& to, const string& value);
