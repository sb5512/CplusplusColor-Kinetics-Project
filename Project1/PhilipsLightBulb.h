#pragma once
#include<iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <windows.h>

using namespace std;

class PhilipsLightBulb
{
private:
	SOCKET SendSocket;
	string destinationAddress;
	unsigned short Port;
	int lightNumber;
	// Helper functions
	void stringToUpper(string &s);
	std::string string_to_hex(char* input); //converts given string to hex value
	void sendReceivePackets(vector<string> packet);
	void createSocketForConnection();
public:
	PhilipsLightBulb(string serialNumber, string destinationAddress, unsigned short Port, SOCKET SendSocket, int lightNumber);
	~PhilipsLightBulb();
	void turnOn();
	void turnOff();
};

