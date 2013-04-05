#ifndef BLOBTRACKERLISTENER_H
#define BLOBTRACKERLISTENER_H

#include <ofxCvConstants_Track.h>


class BlobTrackerListener : public ofxCvBlobListener
{
    public:
        BlobTrackerListener();
        virtual ~BlobTrackerListener();

        void blobOff( int x, int y, int id, int order, int _age ) {

            if (_age >= 30) bHighFiveStatus = true;

        }

        void blobOn( int x, int y, int id, int order );
        void blobMoved( int x, int y, int id, int order );

        bool shouldHighFiveBeGiven() {
            return bHighFiveStatus;
        }
        void resetHighFiveStatus() {
            bHighFiveStatus = false;
        }

    protected:
    private:
        bool    bHighFiveStatus;

};

#endif // BLOBTRACKERLISTENER_H
