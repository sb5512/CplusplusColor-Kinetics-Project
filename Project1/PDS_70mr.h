#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#include<iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "PhilipsLightBulb.h"

using namespace std;

class PDS_70mr
{
private:
	SOCKET SendSocket = INVALID_SOCKET;
	vector<string> serialNumberLights;
	string destinationAddress;
	unsigned short Port;
	
	//Helper functions
	void createSocketForConnection(); // Creates socket on automatically obtained port number for local address and binds the socket. 
	void initialiseLights();
	void discoverLights();
	string PDS_70mr::getRemainingZeros(int size);

public:
	/* 
	   Takes serial number for each lights in order you want it to program to. 
	   Takes Starting address. Usually an Integer as 1 , 2 , 3. Remember each light will need 3 channels for one light bulb. Hence Programming happens in shift of 3. so 1 and then 4 etc
	   Takes a boolean program i.e. if you wish to program the lights initially.
	   Takes the PDS_70mr destination address.. Note this is usually 10.30.1.... 
	   Takes the port. Also usually 6038
	 */
	PDS_70mr(vector<string> serialNumberLights, int startingAddress, bool program, string destinationAddress, unsigned short Port); 
	~PDS_70mr();	
	vector<PhilipsLightBulb*> Lights; // vector of object light bulbs i.e. PhilipsLightBulb class 
	void PDS_70mr::turnOn(int lightnumber, int Red, int Green, int Blue);
	void turnOff();
	void turnOff(int lightNumber);
	void programLights(int startingAddress);
};



