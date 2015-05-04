#ifndef CANON650D_H
#define CANON650D_H

#include "EDSDK.h"
#include "EDSDKErrors.h"
#include "EDSDKTypes.h"

// START OF STATIC FUNCTIONS
static void easyRelease(EdsBaseRef &ref){
	if (ref != NULL){
		EdsRelease(ref);
		ref = NULL;
	}
}
static EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event,
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
static EdsError EDSCALLBACK handlePropertyEvent(EdsPropertyEvent event,
	EdsPropertyID property, EdsUInt32 inParam,
	EdsVoid * context)
{

	// do something
	return EDS_ERR_OK;
}
static EdsError EDSCALLBACK handleStateEvent(EdsStateEvent event,
	EdsUInt32 parameter,
	EdsVoid * context)
{
	// do something
	return EDS_ERR_OK;
}

// END OF STATIC FUNCTIONS

class Canon650DCamera {
	private:
		EdsCameraRef camera; // stores camera object obtained from getCamera function
		EdsError initialiseErr = EDS_ERR_OK;
		bool isSDKLoaded;

		EdsError downloadImage(EdsDirectoryItemRef directoryItem); // Helper function for downloadLastImage
		EdsError getFirstCamera(EdsCameraRef *camera);
	
	public:

		Canon650DCamera();
		~ Canon650DCamera();
		EdsCameraRef getCamera(); // Gets the installed camera as an object
		EdsError getInitialiseErr(); // Returns an error if any task you want to perform is invalid

		EdsError takePicture(EdsCameraRef camera); // takes the picture 
		bool downloadLastImage(EdsCameraRef camera); // Downloads the last image in the camera 

		EdsError getTv(EdsCameraRef camera, EdsUInt32 *Tv);
		EdsError getTvDesc(EdsCameraRef camera, EdsPropertyDesc *TvDesc);
		EdsError setTv(EdsCameraRef camera, EdsUInt32 TvValue);



		//testing this one 
		EdsError setTvs(EdsCameraRef camera);
		// testing this one ends
		
		EdsError startLiveview(EdsCameraRef camera);
		EdsError endLiveview(EdsCameraRef camera);

		EdsError BulbStart(EdsCameraRef camera);
		EdsError BulbStop(EdsCameraRef camera);
		EdsError downloadEvfData(EdsCameraRef camera);		
};
#endif

