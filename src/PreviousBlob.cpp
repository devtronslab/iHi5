#include "PreviousBlob.h"

PreviousBlob::PreviousBlob(int _theId)
{
    blobId = _theId;
    numFrames = 0;
    bBlobFound = true;
}

PreviousBlob::~PreviousBlob()
{
    //dtor
}


int PreviousBlob::getId() {

    return blobId;

}

int PreviousBlob::searchBlobinthisFrame(vector<ofxCvTrackedBlob> curFrameBlobs) {

    bBlobFound = false;

    for (int i = 0; i < curFrameBlobs.size(); i++) {

        if (blobId == curFrameBlobs[i].id) {
            bBlobFound = true;
            return blobId;
        }
    }
    return -1;
}

bool PreviousBlob::bBlobStillHere() {

    return bBlobFound;

}

void PreviousBlob::updateFrameNum() {

    numFrames++;

}

int PreviousBlob::getLifetime() {

    return numFrames;

}
