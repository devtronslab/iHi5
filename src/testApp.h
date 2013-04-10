#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "ofxCvBlobTracker.h"
#include "ofxCvTrackedBlob.h"
#include "ofxCvConstants_Track.h"

#include "TextField.h"

#include "ofxOsc.h"

#include "BlobTrackerListener.h"

#define PORT 12345

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void handDetectHaar(ofxCvGrayscaleImage imageToDetect);
		bool notFaceCheck(ofxCvGrayscaleImage theImage);
		void blobDisappeared(int &_age);
		void giveHighFive();
		void endHighFive();
		void sendHighFiveMessage();
		void incomingHighFiveProtocol();
		void checkForOscMessage();
		void zeroHand();

		ofTrueTypeFont                  font;
		ofSerial                        serial;
		vector<ofSerialDeviceInfo>      devices;

		ofVideoGrabber                  vidGrabber;
		int                             camWidth;
		int                             camHeight;
		ofxCvColorImage                 colorImage;
		ofxCvGrayscaleImage             grayImage;


        ofxCvHaarFinder                 finder;
        ofxCvHaarFinder                 faceFinder;

        ofxCvBlobTracker                handTracker;
        BlobTrackerListener             *theListener;


        bool                            bFullscreen;

        int                             detectedObjects;
        int                             handsFound;
        int                             maxFrameNum;

        TextField                       *USB_Address;
        TextField                       *networkAddress;

        bool                            handConnected;
        bool                            highFiveSent;

        ofxOscSender                    sender;
        ofxOscReceiver                  receiver;

};
