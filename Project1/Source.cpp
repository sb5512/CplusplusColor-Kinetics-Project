#include <iostream>
#include "canon650D.h"
#include <Windows.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
using namespace std;

void changeChannel();
void updateChannel();

int king;
void setK(int k){
	king = k;
}
int getK(){
	return king;
}
int main()
{
	//int hello;	
	//cin >> hello;
	king = 0;

	/// this part is for taking images
	//int jkl = 0;
	//while (jkl < 3){

		INPUT input = { 0 };
		input.type = INPUT_MOUSE;

		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

		input.mi.dx = (long)10000;
		input.mi.dy = (long)49300;
		SendInput(1, &input, sizeof(INPUT));

		int  j = 0;
		while (j < 10){
			j++;
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
			input.mi.mouseData = 0;
			input.mi.dwExtraInfo = NULL;
			input.mi.time = 0;
			SendInput(1, &input, sizeof(INPUT));
		}

		Sleep(1000);
		std::thread t1(changeChannel);

		t1.join();

		//updateChannel();
		// This part is to update channel
		
		/*
		INPUT inputers = { 0 };
		inputers.type = INPUT_MOUSE;

		inputers.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

		inputers.mi.dx = (long)28000;
		inputers.mi.dy = (long)16300;
		SendInput(1, &inputers, sizeof(INPUT));

		j = 0;

		while (j < 50){
			j++;
			inputers.type = INPUT_MOUSE;
			inputers.mi.dwFlags = (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
			inputers.mi.mouseData = 0;
			inputers.mi.dwExtraInfo = NULL;
			inputers.mi.time = 0;
			SendInput(1, &inputers, sizeof(INPUT));
		}
		//int mk;
		//cin >> mk;
		jkl++;
		Sleep(1000);
	}*/


	/*
	Canon650DCamera c;
	if (c.getInitialiseErr() == EDS_ERR_OK){
		EdsUInt32 * tv = NULL;
		EdsError err1 = NULL;

		//err1 = getTv(camera, tv);
		//startLiveview(camera);

		c.setTvs(c.getCamera());

        
		
		
		int takePic;
		
		cout <<endl<< "Press 1 to take picture" << endl;
		cout << "Press 0 to end camera process" << endl;

		do {
			cin >> takePic;
			if (takePic == 1){
				c.takePicture(c.getCamera());
			}
		} while (takePic != 0);

		//c.downloadLastImage(c.getCamera());
		//c.startLiveview(c.getCamera());
		
	}*/
}

void changeChannel(){

	// This part is to change the channel
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;

	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

	Input.mi.dx = (long)1700;
	Input.mi.dy = (long)19300;

	SendInput(1, &Input, sizeof(INPUT));


	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
	Input.mi.mouseData = 0;
	Input.mi.dwExtraInfo = NULL;
	Input.mi.time = 0;
	SendInput(1, &Input, sizeof(INPUT));

	keybd_event(VK_RIGHT, 0, 0, 0);
	keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
	Sleep(1000);
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	//Sleep(1000);
}


void updateChannel(){
	INPUT inputer = { 0 };
	inputer.type = INPUT_MOUSE;

	inputer.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

	inputer.mi.dx = (long)17000;
	inputer.mi.dy = (long)1200;
	SendInput(1, &inputer, sizeof(INPUT));

	int j = 0;

	while (j < 1){
		j++;
		inputer.type = INPUT_MOUSE;
		inputer.mi.dwFlags = (MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
		inputer.mi.mouseData = 0;
		inputer.mi.dwExtraInfo = NULL;
		inputer.mi.time = 0;
		SendInput(1, &inputer, sizeof(INPUT));
	}
}