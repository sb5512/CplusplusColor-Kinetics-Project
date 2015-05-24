#include "UTILS.h"

string	UTILS::decimalToHex(int value){
	string returnString;
	std::stringstream ss;
	ss << std::hex << value; // int decimal_value
	std::string res(ss.str());
	if (res.size() == 1) {
		returnString = "0" + res;// Because hex value less then 10 will have one digit value	
		return returnString;
	}
	else { return res; }	
}

string UTILS::string_to_hex(char* input) //converts given string to hex value
{
	//cout << input;
	static const char* const lut = "0123456789ABCDEF";
	size_t len = strlen(input);
	//if (len & 1) throw std::invalid_argument("odd length");

	std::string output;
	output.reserve(len / 2);
	for (size_t i = 0; i < len - 1; i += 2)
	{
		char a = input[i];
		const char* p = std::lower_bound(lut, lut + 16, a);
		if (*p != a) throw std::invalid_argument("not a hex digit");

		char b = input[i + 1];
		const char* q = std::lower_bound(lut, lut + 16, b);
		if (*q != b) throw std::invalid_argument("not a hex digit");

		output.push_back(((p - lut) << 4) | (q - lut));
	}
	//cout << output;
	return output;
}


void UTILS::stringToUpper(string &s)
{
	for (unsigned int l = 0; l < s.length(); l++)
	{
		s[l] = toupper(s[l]);
	}
}