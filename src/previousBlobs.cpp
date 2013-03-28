#include "previousBlobs.h"

previousBlobs::previousBlobs(ofPoint blobsCenter)
{
    theCenter = blobsCenter;
    numFrames = 0;
    bBlobFound = false;
}

previousBlobs::~previousBlobs()
{
    //dtor
}
