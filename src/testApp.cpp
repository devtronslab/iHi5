#include "testApp.h"

int fingerTipHits = 0;
int webbingHits = 0;

//--------------------------------------------------------------
void testApp::setup(){

    camWidth    = 320;
    camHeight   = 240;

    bFullscreen = false;

    searchRes   = 5;
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

    string zeroString = "returnzero";
    zeroString += "\n";
    for (int i = 0; i < zeroString.length(); i++) {
        serial.writeByte(zeroString[i]);
    }

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

    ofLog(OF_LOG_NOTICE, ofToString(prevHaarBlobs.size()));
    ofLog(OF_LOG_NOTICE, ofToString(finder.blobs.size()));

}

//--------------------------------------------------------------
void testApp::draw(){

    ofPushMatrix();
    ofScale(4, 4);
    grayImage.draw(5,5);
    ofNoFill();
    for (int i = 0; i < finder.blobs.size(); i++) {

        ofRectangle cur = finder.blobs[i].boundingRect;
        ofRect(cur.x, cur.y, cur.width, cur.height);

    }
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
    string endString = "endhighfive";
    endString += "\n";

    for (int i = 0; i < initString.length(); i++) {
        serial.writeByte(initString[i]);
    }

    ofSleepMillis(5000);

    for (int i = 0; i < endString.length(); i++) {
        serial.writeByte(endString[i]);
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

    finder.findHaarObjects(imageToDetect);

//    for (int i = 0; i < finder.blobs.size(); i++) {
//
//        for (int j = 0; j < prevHaarBlobs.size(); j++) {
//
//            if (finder.blobs[i].centroid.x > prevHaarBlobs[i].theCenter.x - searchRes &&
//                    finder.blobs[i].centroid.x < prevHaarBlobs[i].theCenter.x + searchRes &&
//                    finder.blobs[i].centroid.y > prevHaarBlobs[i].theCenter.y - searchRes &&
//                    finder.blobs[i].centroid.y < prevHaarBlobs[i].theCenter.y + searchRes) {
//
//                prevHaarBlobs[j].bBlobFound = true;
//                break;
//
//            }
//
//        }
//
//        prevHaarBlobs.push_back(previousBlobs(finder.blobs[i].centroid));
//
//    }
//
//    for (prevBlobIt = prevHaarBlobs.begin(); prevBlobIt != prevHaarBlobs.end(); prevBlobIt++) {
//
//        previousBlobs tmpBlob = *prevBlobIt;
//
//        if (tmpBlob.bBlobFound) {
//
//            tmpBlob.bBlobFound = false;
//            tmpBlob.numFrames++;
//
//        }
//        else {
//
//            prevHaarBlobs.erase(prevBlobIt);
//
//        }
//
//    }

}

void testApp::exit() {

    string zeroString = "returnzero";
    zeroString += "\n";
    for (int i = 0; i < zeroString.length(); i++) {
        serial.writeByte(zeroString[i]);
    }

}
