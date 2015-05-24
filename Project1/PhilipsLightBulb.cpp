#include "PhilipsLightBulb.h"
#include "UTILS.h"

// Lightbulb Subclass
PhilipsLightBulb::PhilipsLightBulb(string serialNumber, string destinationAddress, unsigned short Port, SOCKET SendSocket, int lightNumber)
{
	this->SendSocket = SendSocket;
	this->destinationAddress = destinationAddress;
	this->Port = Port;
	this->lightNumber = lightNumber;
	for (int i = 0; i < 3; i++){
		currentRGB.push_back(0);
	}
}

void PhilipsLightBulb::turnOn(int Red , int Green , int Blue , vector<int> allLightRGB)
{
	string header = "0401dc4a010001010000000000000000ffffffff00"; // TODO MAKE IT GLOBAL
	string packet = header;
	for (int i = 0; i < lightNumber * 3; i++){
		packet = packet + UTILS::decimalToHex(allLightRGB[i]);
	}
	packet = packet + UTILS::decimalToHex(Red) + UTILS::decimalToHex(Green) + UTILS::decimalToHex(Blue);

	int remainingRGB = 12 - lightNumber * 3;
	for (int j = 3; j < remainingRGB; j++){
		packet = packet + UTILS::decimalToHex(allLightRGB[lightNumber * 3 + j]);
	}

	for (int byteCounter = packet.size(); byteCounter < 1024; byteCounter++){
		packet = packet + "0";
	}
	vector<string> pkt;
	pkt.push_back( packet);
	sendReceivePackets(pkt);
	setRGB(Red, Green, Blue);
}

vector<int> PhilipsLightBulb:: getRGB(){
	return currentRGB;
}
void PhilipsLightBulb::setRGB(int Red , int Green , int Blue){
	currentRGB[0] = Red;
	currentRGB[1] = Green;
	currentRGB[2] = Blue;
}
PhilipsLightBulb::~PhilipsLightBulb()
{
}

void PhilipsLightBulb ::sendReceivePackets(vector<string> packet){

	//---------------------------------------------
	//Getting the packet to the format understood by socket during sending
	for (int i = 0; i < packet.size(); i++){
		UTILS::stringToUpper(packet[i]);
		char * x = &packet[i][0u]; // converting string to char *
		//cout << packet[i];
		packet[i] = UTILS::string_to_hex(x);
	}

	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "192.168.136.1")
	// and the specified port number.
	sockaddr_in DestAddr;
	DestAddr.sin_family = AF_INET;
	DestAddr.sin_port = htons(Port);
	DestAddr.sin_addr.s_addr = inet_addr(destinationAddress.c_str());

	//---------------------------------------------
	// Send a datagram to the receiver
	int sendResult;
	size_t pkt_length = 512;
	wprintf(L"Sending a datagram to the given destination address...\n");
	int j = 0;
	while (j<packet.size()){
		const char * pk1 = packet[j].c_str();
		cout << pk1;
		Sleep(200);
		sendResult = sendto(SendSocket,
			pk1, pkt_length, 0, (SOCKADDR *)& DestAddr, sizeof (DestAddr));
		j++;
	}
	if (sendResult == SOCKET_ERROR) {
		wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();
		
		return;
	}
	cout << "Packet has been sent." << endl;

	/*
	//-----------------------------------------------
	// Call the recvfrom function to receive datagrams
	// on the bound socket.
	char RecvBuf[1024];
	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof (SenderAddr);
	wprintf(L"Receiving datagrams...\n");
	iResult = recvfrom(SendSocket,
	RecvBuf, BufLen, 0, (SOCKADDR *)& SenderAddr, &SenderAddrSize);
	std::cout << "recieved" << iResult;
	if (iResult == SOCKET_ERROR) {
	wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
	}
	*/
}