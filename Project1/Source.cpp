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
EdsError takePicture(EdsCameraRef camera);
EdsError getTv(EdsCameraRef camera, EdsUInt32 *Tv);
EdsError startLiveview(EdsCameraRef camera);
EdsError downloadImage(EdsDirectoryItemRef directoryItem);


bool downloadLastImage(EdsCameraRef camera){

	EdsVolumeRef 		theVolumeRef = NULL;
	EdsDirectoryItemRef	dirItemRef_DCIM = NULL;
	EdsDirectoryItemRef	dirItemRef_Sub = NULL;
	EdsDirectoryItemRef	dirItemRef_Image = NULL;

	EdsDirectoryItemInfo dirItemInfo_Image;

	EdsError err = EDS_ERR_OK;
	EdsUInt32 Count = 0;
	bool success = false;

	//get the number of memory devices
	err = EdsGetChildCount(camera, &Count);
	if (Count == 0){
		printf("Memory device not found\n");
		err = EDS_ERR_DEVICE_NOT_FOUND;
		return false;
	}

	// Download Card No.0 contents
	err = EdsGetChildAtIndex(camera, 0, &theVolumeRef);
	//        if ( err == EDS_ERR_OK ){
	//            printf("getting volume info\n");
	//            //err = EdsGetVolumeInfo( theVolumeRef, &volumeInfo ) ;
	//        }

	//Now lets find out how many Folders the volume has
	if (err == EDS_ERR_OK){
		err = EdsGetChildCount(theVolumeRef, &Count);

		if (err == EDS_ERR_OK){

			//Lets find the folder called DCIM
			bool bFoundDCIM = false;
			for (int i = 0; i < Count; i++){
				err = EdsGetChildAtIndex(theVolumeRef, i, &dirItemRef_DCIM);
				if (err == EDS_ERR_OK){
					EdsDirectoryItemInfo dirItemInfo;
					err = EdsGetDirectoryItemInfo(dirItemRef_DCIM, &dirItemInfo);
					if (err == EDS_ERR_OK){
						string folderName = dirItemInfo.szFileName;
						if (folderName == "DCIM"){
							bFoundDCIM = true;
							printf("Found the DCIM folder at index %i\n", i);
							break;
						}
					}
				}
				//we want to release the directories that don't match
				//TODO easyRelease(dirItemRef_DCIM);
			}

			//This is a bit silly.
			//Essentially we traverse into the DCIM folder, then we go into the last folder in there, then we
			//get the last image in last folder.
			if (bFoundDCIM && dirItemRef_DCIM != NULL){
				//now we are going to look for the last folder in DCIM
				Count = 0;
				err = EdsGetChildCount(dirItemRef_DCIM, &Count);

				bool foundLastFolder = false;
				if (Count > 0){
					int lastIndex = Count - 1;

					EdsDirectoryItemInfo dirItemInfo_Sub;

					err = EdsGetChildAtIndex(dirItemRef_DCIM, lastIndex, &dirItemRef_Sub);
					err = EdsGetDirectoryItemInfo(dirItemRef_Sub, &dirItemInfo_Sub);

					printf("Last Folder is %s \n", dirItemInfo_Sub.szFileName);

					EdsUInt32 jpgCount = 0;
					err = EdsGetChildCount(dirItemRef_Sub, &jpgCount);

					if (jpgCount > 0){
						int latestJpg = jpgCount - 1;

						err = EdsGetChildAtIndex(dirItemRef_Sub, latestJpg, &dirItemRef_Image);
						err = EdsGetDirectoryItemInfo(dirItemRef_Image, &dirItemInfo_Image);

						printf("Latest image is %s \n", dirItemInfo_Image.szFileName);
						success = true;
					}
					else{
						printf("Error - No jpegs inside %s\n", dirItemInfo_Image.szFileName);
					}
				}
				else{
					printf("Error - No subfolders inside DCIM!\n");
				}
			}
		}
	}
	if (success){
		success = downloadImage(dirItemRef_Image);
	}

	/*easyRelease(theVolumeRef);
	easyRelease(dirItemRef_DCIM);
	easyRelease(dirItemRef_Sub);
	easyRelease(dirItemRef_Image);

	postCommand();
	*/
	return success;
}





void applicationRun()
{
	EdsError err = EDS_ERR_OK;
	EdsCameraRef camera = NULL;
	bool isSDKLoaded = false;
	// Initialize SDK
	err = EdsInitializeSDK();
	
	if (err == EDS_ERR_OK)
	{
		cout << endl << "\t\tInitialized SDK..........." << endl;
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
		cout << "\t\t Got Camera..........." << endl;
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
		err = EdsOpenSession(camera);
		if (err = EDS_ERR_OK){
			cout << "Camera loaded..................."<< endl;
		}
		
	}
	/////
	// do something
	
	if (err == EDS_ERR_OK){
		EdsUInt32 * tv = NULL;
		EdsError err1 = NULL;
		err1 = getTv(camera, tv);
		//startLiveview(camera);
		downloadLastImage(camera);
		cout << "tv" << tv;
		cout << "GOOD job" << endl;
	}
	int takePic;
	cout << "Press 1 to take picture" << endl;
	cout << "Press 0 to end camera process" << endl;

	do {
		cin >> takePic;
		if (takePic == 1){
			takePicture(camera);
		}
	} while (takePic != 0);

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
	exit(0);
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
	cout << "am i Here for downloading image ? ";
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