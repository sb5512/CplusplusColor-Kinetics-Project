#include "PhilipsLightBulb.h"

// Lightbulb Subclass
PhilipsLightBulb::PhilipsLightBulb(string serialNumber, string destinationAddress, unsigned short Port, SOCKET SendSocket, int lightNumber)
{
	this->SendSocket = SendSocket;
	this->destinationAddress = destinationAddress;
	this->Port = Port;
	this->lightNumber = lightNumber;
}

void PhilipsLightBulb::turnOn()
{
	vector<string> pkt;
	pkt.push_back("0401dc4a01000102000000000000000");
	sendReceivePackets(pkt);
}

void PhilipsLightBulb::turnOff(){
	
}

PhilipsLightBulb::~PhilipsLightBulb()
{
}

void PhilipsLightBulb ::sendReceivePackets(vector<string> packet){

	//---------------------------------------------
	//Getting the packet to the format understood by socket during sending
	for (int i = 0; i < packet.size(); i++){
		stringToUpper(packet[i]);
		char * x = &packet[i][0u]; // converting string to char *
		//cout << packet[i];
		packet[i] = string_to_hex(x);
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
		//cout << pk1;
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
	cout << "Packet has been sent. Type anything to end.." << endl;
    int hehe;
	cin >> hehe;

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


std::string PhilipsLightBulb::string_to_hex(char* input)
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


void PhilipsLightBulb::stringToUpper(string &s)
{
	for (unsigned int l = 0; l < s.length(); l++)
	{
		s[l] = toupper(s[l]);
	}
}

