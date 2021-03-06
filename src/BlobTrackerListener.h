#ifndef BLOBTRACKERLISTENER_H
#define BLOBTRACKERLISTENER_H

#include <ofxCvConstants_Track.h>
#

class BlobTrackerListener : public ofxCvBlobListener
{
    public:
        BlobTrackerListener();
        virtual ~BlobTrackerListener();

        void blobOff( int x, int y, int id, int order, int _age );
        void blobOn( int x, int y, int id, int order );
        void blobMoved( int x, int y, int id, int order );

        bool shouldHighFiveBeGiven() {
            return bHighFiveStatus;
        }

        void resetHighFiveStatus() {
            bHighFiveStatus = false;
        }

        ofEvent<int>    blobOffEvent;

    protected:
    private:
        bool    bHighFiveStatus;

};

#endif // BLOBTRACKERLISTENER_H
