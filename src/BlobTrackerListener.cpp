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

    if (_age >= 30) {
        bHighFiveStatus = true;
    }

}

void BlobTrackerListener::blobOn( int x, int y, int id, int order ){

}

void BlobTrackerListener::blobMoved( int x, int y, int id, int order ){

}
