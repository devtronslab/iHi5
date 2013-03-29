#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "previousBlobs.h"

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
		void notFaceCheck(ofxCvGrayscaleImage theImage, previousBlobs handCandidate);
		void giveHighFive();
		void zeroHand();

		ofTrueTypeFont                  font;
		ofSerial                        serial;

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

};
