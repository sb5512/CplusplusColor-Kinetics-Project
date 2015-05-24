#pragma once
#include <iostream>
#include <string>
#include "canon650D.h"
#include "PDS_70mr.h"
using namespace std;

void takeAndDownloadImage(Canon650DCamera* canonCamera);

int main()
{
	vector<string> serialNumberLights55;
	serialNumberLights55.push_back("4304005A");
	serialNumberLights55.push_back("4302437E");
	serialNumberLights55.push_back("43040271");
	serialNumberLights55.push_back("43024403");

	vector<string> serialNumberLights51;
	serialNumberLights51.push_back("4304014D");
	serialNumberLights51.push_back("430242F2");
	serialNumberLights51.push_back("4304016E");
	serialNumberLights51.push_back("43024318");

	// vmnetaddress // 192.168.136.142
	Canon650DCamera* canonCamera = new Canon650DCamera();

	PDS_70mr* pds_70mr51 = new PDS_70mr(serialNumberLights51, 1, FALSE, "10.33.157.51", 6038); // note for pds7- mr we have address as 10.33.157.55 and port 6038
	PDS_70mr* pds_70mr55 = new PDS_70mr(serialNumberLights55, 1, FALSE, "10.33.157.55", 6038); // note for pds7- mr we have address as 10.33.157.55 and port 6038
	unsigned int milliseconds = 1000;
	
	/* Colour Photometric stereo*/
	pds_70mr51->turnOn(2,255,255,255);
	takeAndDownloadImage(canonCamera);
	Sleep(milliseconds);
	pds_70mr51->turnOff(2);

	pds_70mr51->turnOn(4,255, 255, 255);
	takeAndDownloadImage(canonCamera);
	Sleep(milliseconds );
	pds_70mr51->turnOff(4);

	pds_70mr55->turnOn(2, 255, 255, 255);
	takeAndDownloadImage(canonCamera);
	Sleep(milliseconds);
	pds_70mr55->turnOff(2);
	pds_70mr55->turnOn(4, 255, 255, 255);
	takeAndDownloadImage(canonCamera);
	Sleep(milliseconds);
	pds_70mr55->turnOff(4);

	/*White Light Photometric Stereo*/
	pds_70mr51->turnOn(1, 255, 255, 255);
	takeAndDownloadImage(canonCamera);
	Sleep(milliseconds);
	pds_70mr51->turnOff(1);

	pds_70mr51->turnOn(3, 255, 255, 255);
	takeAndDownloadImage(canonCamera);
	Sleep(milliseconds);
	pds_70mr51->turnOff(3);

	pds_70mr55->turnOn(1, 255, 255, 255);
	takeAndDownloadImage(canonCamera);
	Sleep(milliseconds);
	pds_70mr55->turnOff(1);
	pds_70mr55->turnOn(3, 255, 255, 255);
	takeAndDownloadImage(canonCamera);
	Sleep(milliseconds);
	pds_70mr55->turnOff(3);

	/* Discover
	{"0401dc4a01000102000000000000000"}*/

	canonCamera->downloadLastImage(canonCamera->getCamera());
	int wait;
	cin >> wait;
}

void takeAndDownloadImage(Canon650DCamera* canonCamera){
	int camerapictimes = 0;
	while (camerapictimes < 100){
		canonCamera->takePicture(canonCamera->getCamera());
		camerapictimes++;
	}
}