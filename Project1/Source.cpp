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
			handlePropertyEvent, NULL);
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

EdsError getTvDesc(EdsCameraRef camera, EdsPropertyDesc *TvDesc)
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

// Downloading image 

EdsError downloadImage(EdsDirectoryItemRef directoryItem)
{
	EdsError err = EDS_ERR_OK;
	EdsStreamRef stream = NULL;
	// Get directory item information
	EdsDirectoryItemInfo dirItemInfo;
	err = EdsGetDirectoryItemInfo(directoryItem, &dirItemInfo);
	// Create file stream for transfer destination
	if (err == EDS_ERR_OK)
	{
		err = EdsCreateFileStream(dirItemInfo.szFileName,
			kEdsFileCreateDisposition_CreateAlways,
			kEdsAccess_ReadWrite, &stream);
	}
	// Download image
	if (err == EDS_ERR_OK)
	{
		err = EdsDownload(directoryItem, dirItemInfo.size, stream);
	}
	// Issue notification that download is complete
	if (err == EDS_ERR_OK)
	{
		err = EdsDownloadComplete(directoryItem);
	}
	// Release stream
	if (stream != NULL)
	{
		EdsRelease(stream);
		stream = NULL;
	}
	return err;
}

// Getting a file Object
EdsError getVolume(EdsCameraRef camera, EdsVolumeRef * volume)
{
	EdsError err = EDS_ERR_OK;
	EdsUInt32 count = 0;
	// Get the number of camera volumes
	err = EdsGetChildCount(camera, &count);
	if (err == EDS_ERR_OK && count == 0)
	{
		err = EDS_ERR_DIR_NOT_FOUND;
	}
	// Get initial volume
	if (err == EDS_ERR_OK)
	{
		//TODO:: the & volume isnot working
		//err = EdsGetChildAtIndex(camera, 0, &volume);
	}
	return err;
}


EdsError getDCIMFolder(EdsVolumeRef volume, EdsDirectoryItemRef * directoryItem)
{
	EdsError err = EDS_ERR_OK;
	EdsDirectoryItemRef dirItem = NULL;
	EdsDirectoryItemInfo dirItemInfo;
	EdsUInt32 count = 0;
	// Get number of items under the volume
	err = EdsGetChildCount(volume, &count);
	if (err == EDS_ERR_OK && count == 0)
	{
		err = EDS_ERR_DIR_NOT_FOUND;
	}
	// Get DCIM folder

	for (int i = 0; i < count && err == EDS_ERR_OK; i++)
	{
		// Get the ith item under the specifed volume
		if (err == EDS_ERR_OK)
		{
			err = EdsGetChildAtIndex(volume, i, &dirItem);
		}
		// Get retrieved item information
		if (err == EDS_ERR_OK)
		{
			err = EdsGetDirectoryItemInfo(dirItem, &dirItemInfo);
		}
		// Indicates whether or not the retrieved item is a DCIM folder.
		if (err == EDS_ERR_OK)
		{
			if (_stricmp(dirItemInfo.szFileName, "DCIM") == 0 &&
				dirItemInfo.isFolder == true)
			{
				directoryItem = &dirItem;
				break;
			}
		}
		// Release retrieved item
		if (dirItem){
			EdsRelease(dirItem);
			dirItem = NULL;

		}
	}
	return err;
}

EdsError takePicture(EdsCameraRef camera)
{
	return EdsSendCommand(camera , kEdsCameraCommand_TakePicture, 0);
}

//During bulb shooting

EdsError BulbStart(EdsCameraRef camera)
{
	EdsError err;
	bool locked = false;
	err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UILock, 0);
	if (err == EDS_ERR_OK)
	{
		locked = true;
	}
	if (err == EDS_ERR_OK)
	{
		err = EdsSendCommand(camera, kEdsCameraCommand_BulbStart, 0);
	}
	if (err != EDS_ERR_OK && locked)
	{
		err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);
	}
	return err;
}
EdsError BulbStop(EdsCameraRef camera)
{
	EdsError err;
	err = EdsSendCommand(camera, kEdsCameraCommand_BulbEnd, 0);
	EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);
	return err;
}

// Live view
EdsError startLiveview(EdsCameraRef camera)
{
	EdsError err = EDS_ERR_OK;
	// Get the output device for the live view image
	EdsUInt32 device;
	err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	// PC live view starts by setting the PC as the output device for the live view image.
	if (err == EDS_ERR_OK)
	{
		device |= kEdsEvfOutputDevice_PC;
		err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	}
	// A property change event notification is issued from the camera if property settings are made successfully.
	// Start downloading of the live view image once the property change notification arrives.
	return err;
}
EdsError downloadEvfData(EdsCameraRef camera)
{
	EdsError err = EDS_ERR_OK;
	EdsStreamRef stream = NULL;
	EdsEvfImageRef evfImage = NULL;
	// Create memory stream.
	err = EdsCreateMemoryStream(0, &stream);
	// Create EvfImageRef.
	if (err == EDS_ERR_OK)
	{
		err = EdsCreateEvfImageRef(stream, &evfImage);
	}
	// Download live view image data.
	if (err == EDS_ERR_OK)
	{
		err = EdsDownloadEvfImage(camera, evfImage);
	}
	// Get the incidental data of the image.
	if (err == EDS_ERR_OK)
	{
		// Get the zoom ratio
		EdsUInt32 zoom;
		EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomPosition, 0, sizeof(zoom), &zoom);
		// Get the focus and zoom border position
		EdsPoint point;
		EdsGetPropertyData(evfImage , kEdsPropID_Evf_ZoomPosition, 0, sizeof(point), &point);
	}
	//
	// Display image
	//
	// Release stream
	if (stream != NULL)
	{
		EdsRelease(stream);
		stream = NULL;
	}
	// Release evfImage
	if (evfImage != NULL)
	{
		EdsRelease(evfImage);
		evfImage = NULL;
	}
	return err;
}
EdsError endLiveview(EdsCameraRef camera)
{
	EdsError err = EDS_ERR_OK;
	// Get the output device for the live view image
	EdsUInt32 device;
	err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	// PC live view ends if the PC is disconnected from the live view image output device.
	if (err == EDS_ERR_OK)
	{
		device &= ~kEdsEvfOutputDevice_PC;
		err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
	}
	return err;
}


int main()
{
	int hello;
	applicationRun();
	cin >> hello;
}