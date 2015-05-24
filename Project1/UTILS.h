#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class UTILS
{
public:
	static string decimalToHex(int value);
	static string string_to_hex(char* input); //converts given string to hex value
	static void stringToUpper(string &s);
};
