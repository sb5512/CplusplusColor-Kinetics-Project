#include <iostream>
#include "canon650D.h"
using namespace std;

int main()
{
	Canon650DCamera c;
	if (c.getInitialiseErr() == EDS_ERR_OK){
		EdsUInt32 * tv = NULL;
		EdsError err1 = NULL;

		//err1 = getTv(camera, tv);
		//startLiveview(camera);


		int takePic;
		
		cout <<endl<< "Press 1 to take picture" << endl;
		cout << "Press 0 to end camera process" << endl;

		do {
			cin >> takePic;
			if (takePic == 1){
				c.takePicture(c.getCamera());
			}
		} while (takePic != 0);

		c.downloadLastImage(c.getCamera());
		c.startLiveview(c.getCamera());
	}
}