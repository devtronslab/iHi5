#include "testApp.h"

int fingerTipHits = 0;
int webbingHits = 0;

//--------------------------------------------------------------
void testApp::setup(){

    camWidth    = 320;
    camHeight   = 240;

    bFullscreen = false;

    handsFound  = 0;

    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);

    colorImage.allocate(camWidth, camHeight);
    grayImage.allocate(camWidth, camHeight);

    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);

    font.loadFont("DIN.otf", 20);

    serial.enumerateDevices();
    serial.setup("/dev/ttyACM0", 9600);

    finder.setScaleHaar(1.5);
    finder.setup("Hand.Cascade.1.xml");

}

//--------------------------------------------------------------
void testApp::update(){

    bool bNewFrame;

    handsFound = 0;
    fingerTipHits   = 0;

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();

    if (bNewFrame) {

        colorImage.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
        grayImage = colorImage;

        handDetectHaar(grayImage);

    }

}

//--------------------------------------------------------------
void testApp::draw(){

    ofPushMatrix();
    ofScale(4, 4);
    grayImage.draw(5,5);
    ofPopMatrix();

    font.drawString("frame rate:", 20, (ofGetWindowHeight() - 50));
    font.drawString(ofToString(ofGetFrameRate()), 400, (ofGetWindowHeight() - 50));

    font.drawString("number of objects: ", 20, (ofGetWindowHeight() - 100));
    font.drawString(ofToString(detectedObjects), 400, (ofGetWindowHeight() - 100));
    font.drawString("number of hands found:", 20, (ofGetWindowHeight() - 150));
    font.drawString(ofToString(handsFound), 400, (ofGetWindowHeight() - 150));

    ofSetColor(255,255,255);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key) {
        case 'f':
            bFullscreen = !bFullscreen;
            ofSetFullscreen(bFullscreen);
            break;
    }
    serial.writeByte(key);


}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    string initString = "inithighfive";
    initString += "\n";

    for (int i = 0; i < initString.length(); i++) {
        serial.writeByte(initString[i]);

    }

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

void testApp::handDetectHaar(ofxCvGrayscaleImage imageToDetect) {



}
