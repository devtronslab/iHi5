#include "BlobTrackerListener.h"

BlobTrackerListener::BlobTrackerListener()
{
    bHighFiveStatus = false;
}

BlobTrackerListener::~BlobTrackerListener()
{
    //dtor
}

void BlobTrackerListener::blobOff( int x, int y, int id, int order, int _age ) {

    ofNotifyEvent(blobOffEvent,_age,this);

}

void BlobTrackerListener::blobOn( int x, int y, int id, int order ){

}

void BlobTrackerListener::blobMoved( int x, int y, int id, int order ){

}
