#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "ofxCvTrackedBlob.h"

#include "previousBlobs.h"

#include "textField.h"

#include "ofxOsc.h"

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
		bool notFaceCheck(ofxCvGrayscaleImage theImage, previousBlobs handCandidate);
		void giveHighFive();
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

		int                             searchRes;

		ofxCvColorImage                 colorImage;
		ofxCvGrayscaleImage             grayImage;

        ofxCvHaarFinder                 finder;
        ofxCvHaarFinder                 faceFinder;
        vector<previousBlobs>           prevHaarBlobs;
        vector<previousBlobs>::iterator prevBlobIt;

        bool                            bFullscreen;

        int                             detectedObjects;
        int                             handsFound;
        int                             maxFrameNum;

        textField                       *USB_Address;
        textField                       *networkAddress;

        bool                            handConnected;

        ofxOscSender                    sender;
        ofxOscReceiver                  receiver;

};
