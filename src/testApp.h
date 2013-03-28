#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

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

		ofTrueTypeFont              font;
		ofSerial                    serial;

		ofVideoGrabber              vidGrabber;
		int                         camWidth;
		int                         camHeight;

		ofxCvColorImage             colorImage;
		ofxCvGrayscaleImage         grayImage;

        ofxCvHaarFinder             finder;

        bool                        bFullscreen;

        int                         detectedObjects;
        int                         handsFound;

};
