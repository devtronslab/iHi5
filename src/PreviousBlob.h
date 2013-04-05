#ifndef PREVIOUSBLOB_H
#define PREVIOUSBLOB_H

#include "ofMain.h"

#include "ofxCvBlobTracker.h"

class PreviousBlob
{
    public:
        PreviousBlob(int _theId);
        virtual ~PreviousBlob();

        int         getId();
        int         searchBlobinthisFrame(vector<ofxCvTrackedBlob> curFrameBlobs);
        bool        bBlobStillHere();
        void        updateFrameNum();
        int         getLifetime();


    protected:
    private:
        bool        bBlobFound;
        int         blobId;
        int         numFrames;
};

#endif // PREVIOUSBLOB_H
