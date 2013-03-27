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
		void handDetect(ofxCvBlob contour);
		void handDetectHaar(ofxCvBlob contourDealy);
		void smoothContour(ofxCvBlob blobby);

		ofTrueTypeFont              font;
		ofSerial                    serial;
		string                      keyToDisplay;

		ofVideoGrabber              vidGrabber;
		int                         camWidth;
		int                         camHeight;

		ofxCvColorImage             colorImage;
		ofxCvGrayscaleImage         grayImage;
		vector<ofxCvGrayscaleImage> backgroundImg;
		vector<ofxCvGrayscaleImage>::iterator it;
		ofxCvGrayscaleImage         backgroundImgAg;
		ofxCvGrayscaleImage         differenceImg;
		ofxCvContourFinder          contourFinder;

		int                         threshold;
		int                         blurLevel;
		int                         blockSize;
		bool                        bGaussian;
		bool                        bLearnBG;
        bool                        bAdaptive;
        bool                        bFullscreen;

        int                         handsFound;
        int                         fingerTipThresh;
        int                         webbingThresh;
        int                         detectResolution;

        ofxCvHaarFinder             finder;

};
