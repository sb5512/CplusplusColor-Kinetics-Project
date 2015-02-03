#include <iostream>
#include "EDSDK.h"
#include "EDSDKErrors.h"
#include "EDSDKTypes.h"
using namespace std;
EdsError getFirstCamera(EdsCameraRef *camera);
EdsError EDSCALLBACK handlePropertyEvent(EdsPropertyEvent event,
	EdsPropertyID property, EdsUInt32 inParam,
	EdsVoid * context);
EdsError EDSCALLBACK handleStateEvent(EdsStateEvent event,
	EdsUInt32 parameter,
	EdsVoid * context);
EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event,
	EdsBaseRef object,
	EdsVoid * context);


void applicationRun()
{
	EdsError err = EDS_ERR_OK; 
	EdsCameraRef camera = NULL;
	bool isSDKLoaded = false;
	// Initialize SDK
	err = EdsInitializeSDK();
	cout << endl << "\t\tInitialized SDK with Error Code " << err;
	if (err == EDS_ERR_OK)
	{
		isSDKLoaded = true;
	}
	// Get first camera
	if (err == EDS_ERR_OK)
	{
		err = getFirstCamera(&camera);
	}
	// Set event handler
	if (err == EDS_ERR_OK)
	{
		err = EdsSetObjectEventHandler(camera, kEdsObjectEvent_All,
			handleObjectEvent, NULL);
	}
	// Set event handler
	if (err == EDS_ERR_OK)
	{
		err = EdsSetPropertyEventHandler(camera, kEdsPropertyEvent_All,
			handlePropertyEvent , NULL);
	}
	// Set event handler
	if (err == EDS_ERR_OK)
	{
		err = EdsSetCameraStateEventHandler(camera, kEdsStateEvent_All,
			handleStateEvent, NULL);
	}

	// Open session with camera
	if (err == EDS_ERR_OK)
	{
		cout << "DO I NOT GET HERE"; 
		err = EdsOpenSession(camera);
	}
	/////
	// do something
	cout << "DO I GET HERE";
	////
	// Close session with camera
	if (err == EDS_ERR_OK)
	{
		err = EdsCloseSession(camera);
	}
	// Release camera
	if (camera != NULL)
	{
		EdsRelease(camera);
	}
	// Terminate SDK
	if (isSDKLoaded)
	{
		EdsTerminateSDK();
	}
}

EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event,
	EdsBaseRef object,
	EdsVoid * context)
{
	// do something
	/*
	switch(event)
	{
	case kEdsObjectEvent_DirItemRequestTransfer:
	downloadImage(object);
	break;
	default:
	break;
	}
	*/
	// Object must be released
	if (object)
	{
		EdsRelease(object);
	}
	return EDS_ERR_OK; 
}
EdsError EDSCALLBACK handlePropertyEvent(EdsPropertyEvent event,
	EdsPropertyID property, EdsUInt32 inParam,
	EdsVoid * context)
{
	
	// do something
	return EDS_ERR_OK;
}
EdsError EDSCALLBACK handleStateEvent(EdsStateEvent event,
	EdsUInt32 parameter,
	EdsVoid * context)
{
	// do something
	return EDS_ERR_OK;
}

EdsError getFirstCamera(EdsCameraRef *camera)
{
	EdsError err = EDS_ERR_OK;
	EdsCameraListRef cameraList = NULL;
	EdsUInt32 count = 0;
	// Get camera list
	err = EdsGetCameraList(&cameraList);
	// Get number of cameras
	if (err == EDS_ERR_OK)
	{
		err = EdsGetChildCount(cameraList, &count);
		if (count == 0)
		{
			err = EDS_ERR_DEVICE_NOT_FOUND;
			cout << endl << "DUDE NO CAMERA EXISTS MAN CHILLLLLLLLL AND PUT THE CAMERA IN HEHEEH" << endl;
		}
	}
	// Get first camera retrieved
	if (err == EDS_ERR_OK)
	{
		err = EdsGetChildAtIndex(cameraList, 0, camera);
	}
	// Release camera list
	if (cameraList != NULL)
	{
		EdsRelease(cameraList);
		cameraList = NULL;
	}
	return err;
}
EdsError getTv(EdsCameraRef camera, EdsUInt32 *Tv)
{
	EdsError err = EDS_ERR_OK;
	EdsDataType dataType;
	EdsUInt32 dataSize;
	err = EdsGetPropertySize(camera, kEdsPropID_Tv, 0, &dataType, &dataSize);
	if (err == EDS_ERR_OK)
	{
		err = EdsGetPropertyData(camera, kEdsPropID_Tv, 0, dataSize, Tv);
	}
	return err;
}

EdsError getTvDesc(EdsCameraRef camera,  EdsPropertyDesc *TvDesc)
{
	EdsError err = EDS_ERR_OK;
	err = EdsGetPropertyDesc(camera, kEdsPropID_Tv, TvDesc);
	return err;
}

EdsError setTv(EdsCameraRef camera, EdsUInt32 TvValue)
{
	EdsError err = EDS_ERR_OK;
	err = EdsSetPropertyData(camera, kEdsPropID_Tv, 0, sizeof(TvValue), &TvValue);
	return err;
}



int main()
{
	int hello;
	applicationRun();
	cin >> hello;
}