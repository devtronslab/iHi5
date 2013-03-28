#ifndef PREVIOUSBLOBS_H
#define PREVIOUSBLOBS_H

#include "ofMain.h"

class previousBlobs
{
    public:
        previousBlobs(ofPoint blobsCenter);
        virtual ~previousBlobs();

        ofPoint     theCenter;
        int         numFrames;
        bool        bBlobFound;

    protected:
    private:
};

#endif // PREVIOUSBLOBS_H
