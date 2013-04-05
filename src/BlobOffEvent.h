#ifndef BLOBOFFEVENT_H
#define BLOBOFFEVENT_H

#include <ofMain.h>
#include "ofxCvTrackedBlob.h"


class BlobOffEvent : public ofEventArgs
{
    public:

        ofxCvTrackedBlob *disappearingBlob;
        BlobOffEvent() {
            disappearingBlob = NULL;
        }

        static ofEvent <BlobOffEvent> events;

    protected:
    private:
};

#endif // BLOBOFFEVENT_H
