#pragma once
#include<iostream>
#include <windows.h>
#include <vector>
using namespace std;

class PhilipsLightBulb
{
private:
	SOCKET SendSocket;
	string destinationAddress;
	unsigned short Port;
	int lightNumber;
	vector<int> currentRGB;	
	void createSocketForConnection();

public:
	PhilipsLightBulb(string serialNumber, string destinationAddress, unsigned short Port, SOCKET SendSocket, int lightNumber);
	~PhilipsLightBulb();
	void turnOn(int Red, int Green, int Blue, vector<int> allLightRGB);
	void sendReceivePackets(vector<string> packet);
	vector<int> getRGB();
	void setRGB(int Red, int Green, int Blue);
};

