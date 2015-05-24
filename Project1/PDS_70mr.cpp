#include "PDS_70mr.h"
#include "PhilipsLightBulb.h"
#include "UTILS.h"

PDS_70mr::PDS_70mr(vector<string> serialNumberLights, int startingAddress, bool program, string destinationAddress, unsigned short Port)
{
	this->serialNumberLights = serialNumberLights;
	this->destinationAddress = destinationAddress;
	this->Port = Port;
	createSocketForConnection();
	if(program){
		programLights(startingAddress);
	}
	initialiseLights();
}


PDS_70mr::~PDS_70mr()
{
	//---------------------------------------------
	// When the application is finished sending, close the socket.
	wprintf(L"Finished sending. Closing socket.\n");
	int closeResult;
	closeResult = closesocket(SendSocket);
	if (closeResult == SOCKET_ERROR) {
		wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	//---------------------------------------------
	// Clean up and quit.
	wprintf(L"Exiting.\n");
	WSACleanup();
}

void PDS_70mr::discoverLights(){
	vector<string> pkt;
	string packet = "0401dc4a01000102000000000000000";
	for (int byteCounter = packet.size(); byteCounter < 1024; byteCounter++){
		packet = packet + "0";
	}
	pkt.push_back(packet);
	Lights[0]->sendReceivePackets(pkt);
}
void PDS_70mr::programLights(int startingAddress){
	for (int lightCounter = 0; lightCounter < serialNumberLights.size(); lightCounter++){
		vector<string> pkt;
		string p1 = "0401dc4a010001010000000000000000ffffffff7a041becdb5a00";
		p1 = p1 + getRemainingZeros(p1.size());
		for (int k = 0; k < 5; k++){
			pkt.push_back(p1);
		}

		string p2 = "0401dc4a010001010000000000000000ffffffff8b03";
		p2 = p2 + "FF";
		p2 = p2 + "00400001";
		p2 = p2 + serialNumberLights[lightCounter];
		p2 = p2 + "00";
		p2 = p2 + getRemainingZeros(p2.size());
		pkt.push_back(p2);

		string p3 = "0401dc4a010001010000000000000000ffffffff8b03";
		p3 = p3 + UTILS::decimalToHex(255 - (startingAddress + (3 * lightCounter)));
		p3 = p3 + "00410001";
		p3 = p3 + serialNumberLights[lightCounter];
		p3 = p3 + UTILS::decimalToHex((3 * lightCounter) + (startingAddress - 1));
		cout << " What the hello " << UTILS::decimalToHex((3 * lightCounter) + (startingAddress - 1)) << endl;
		p3 = p3 + getRemainingZeros(p3.size());
		pkt.push_back(p3);

		// These values are constant for Philips light bulbs Icolor WR and Icolor
		string p4 = "0401dc4a010001010000000000000000ffffffff830aa000";
		p4 = p4 + getRemainingZeros(p4.size());
		pkt.push_back(p4);
		string p5 = "0401dc4a010001010000000000000000ffffffff8200aa";
		p5 = p5 + getRemainingZeros(p5.size());
		pkt.push_back(p5);
		// until here

		Lights[0]->sendReceivePackets(pkt);
	}
	turnOff();
}

string PDS_70mr::getRemainingZeros(int size){
	string Zeros = "";
	for (int byteCounter = size; byteCounter < 1024; byteCounter++){
		Zeros = Zeros + "0";
	}
	return Zeros;
}

void PDS_70mr::turnOn(int lightnumber, int Red, int Green, int Blue){
	vector<int> allLightRGB;
	for (int i = 0; i < Lights.size(); i++){
		allLightRGB.push_back(Lights[i]->getRGB()[0]);
		allLightRGB.push_back(Lights[i]->getRGB()[1]);
		allLightRGB.push_back(Lights[i]->getRGB()[2]);
	}
	Lights[lightnumber-1]->turnOn(Red, Green, Blue , allLightRGB);
}

void PDS_70mr::turnOff(int lightnumber){
	turnOn(lightnumber, 0, 0, 0);
}

void PDS_70mr::turnOff(){
	vector<string> pkt;
	string packet = "0401dc4a010001010000000000000000ffffffff00";

	for (int byteCounter = packet.size(); byteCounter < 1024; byteCounter++){
		packet = packet + "0";
	}
	pkt.push_back(packet);
	Lights[0]->sendReceivePackets(pkt);
}


void PDS_70mr:: initialiseLights(){
	for (int i = 0; i < serialNumberLights.size(); i++){
		PhilipsLightBulb* bulb1 = new PhilipsLightBulb(serialNumberLights[i], destinationAddress, Port, SendSocket , i);
	    Lights.push_back(bulb1);
	}
}
void PDS_70mr::createSocketForConnection(){
	 //" THIS PART ONWARDS IS FOR SOCKET AND PACKET TRANSFER" 

	//----------------------
	// Initialize Winsock
	int wsaResult;
	WSADATA wsaData;
	wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", wsaResult);
		return;
	}
	cout << "//--------------------------------------------------------------------------//" << endl << endl;
	cout << "|--   Initialised Winsock" << endl;
	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	cout << "|--   Socket created" << endl;
	// -------------------------------------------- -
	// Getting the host name and initialising the local address (i.e your ip address of your computer as a sender)
	char host_name[128] = "";
	gethostname(host_name, sizeof(host_name));
	struct hostent *ent = gethostbyname(host_name);
	struct in_addr ip_addr = *(struct in_addr *)(ent->h_addr);
	printf("|--   Hostname: %s, was resolved to: %s\n",
		host_name, inet_ntoa(ip_addr));

	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = inet_addr("10.1.3.30"); //inet_ntoa(ip_addr));//
	local.sin_port = 0; // choose any

	// -------------------------------------------- -
	// Binding the socket
	int bindResult;
	bindResult = bind(SendSocket, (SOCKADDR *)& local, sizeof (local));
	if (bindResult != 0) {
		wprintf(L"bind failed with error %d\n", WSAGetLastError());
		int wait;
		cin >> wait;
		return;
	}
	cout << "|--   Successful Socket Bind" << endl;
	cout <<endl << "//--------------------------------------------------------------------------//" << endl << endl;
}
