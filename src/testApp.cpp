#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    camWidth    = 320;
    camHeight   = 240;

    bFullscreen = false;

    handsFound  = 0;
    handConnected = false;
    highFiveSent = false;

//    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);

    colorImage.allocate(camWidth, camHeight);
    grayImage.allocate(camWidth, camHeight);

    ofBackground(0);
//    ofSetLogLevel(OF_LOG_VERBOSE);

    font.loadFont("DIN.otf", 16);

    finder.setScaleHaar(1.8);
    finder.setup("Hand.Cascade.1.xml");
    faceFinder.setScaleHaar(1.5);
    faceFinder.setup("haarcascade_frontalface_default.xml");
    maxFrameNum = 0;

    USB_Address = new TextField(300, ofGetWindowHeight() - 91, 300, "/dev/ttyACM0");
    networkAddress = new TextField(300, ofGetWindowHeight() - 116, 300, "128.192.4.254");

    devices = serial.getDeviceList();
    if (serial.setup(USB_Address->fieldString, 9600)) {
        zeroHand();
        handConnected = true;
    }

    sender.setup(networkAddress->fieldString, PORT);
    receiver.setup(PORT);

    theListener = new BlobTrackerListener();
    handTracker.setListener(theListener);

    ofAddListener(theListener->blobOffEvent, this, &testApp::blobDisappeared);

}

//--------------------------------------------------------------
void testApp::update(){

    bool bNewFrame;

    handsFound = 0;

    vidGrabber.update();
    bNewFrame = vidGrabber.isFrameNew();

    if (bNewFrame) {

        colorImage.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
        grayImage = colorImage;
        grayImage.blur(3);

        handDetectHaar(grayImage);
//        if (theListener->shouldHighFiveBeGiven()) {
//            endHighFive();
//            theListener->resetHighFiveStatus();
//            highFiveSent = false;
//            maxFrameNum = 0;
//        }

    }

    USB_Address->update();
    networkAddress->update();

    checkForOscMessage();

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

    for (int i = 0; i < handTracker.blobs.size(); i++) {

        handTracker.blobs[i].draw(0, 0);

    }

    ofPopMatrix();

    ofSetColor(255,255,255);

    font.drawString("frame rate:", 20, (ofGetWindowHeight() - 25));
    font.drawString(ofToString(ofGetFrameRate()), 300, (ofGetWindowHeight() - 25));

    font.drawString("max blob: ", 20, (ofGetWindowHeight() - 50));
    font.drawString(ofToString(maxFrameNum), 300, (ofGetWindowHeight() - 50));

//    for (int i = 0; i < devices.size(); i++) {
//        font.drawString(devices[i].getDeviceName(), 1200, (ofGetWindowHeight() - 50*i));
//
//    }

    ofSetColor(255,255,255);

    font.drawString("USB address for hand: ", 20, (ofGetWindowHeight() - 75));
    USB_Address->draw();
    font.drawString("Network address: ", 20, (ofGetWindowHeight() - 100));
    networkAddress->draw();

    if (handConnected) {
        ofPushMatrix();
            ofSetColor(0,255,0);
            ofFill();
            ofRect(650, (ofGetWindowHeight() - 91), 16, 16);
        ofPopMatrix();
    }
    else {
        ofPushMatrix();
            ofSetColor(255,0,0);
            ofNoFill();
            ofRect(650, (ofGetWindowHeight() - 91), 16, 16);
        ofPopMatrix();
    }

    ofSetColor(255,255,255);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if (USB_Address->isSelected()) {
        if (key == 8) {
            USB_Address->delChar();
        }
        else if (key == 13) {
            if (serial.setup(USB_Address->fieldString, 9600)) {
                zeroHand();
                handConnected = true;
            }
            USB_Address->deactivate();
        }
        else {
            USB_Address->addChar((char)key);
        }
    }
    else if (networkAddress->isSelected()){

        if (key == 8) {
            networkAddress->delChar();
        }
        else if (key == 13) {
            sender.setup(networkAddress->fieldString, PORT);
            networkAddress->deactivate();
        }
        else {
            networkAddress->addChar((char)key);
        }

    }
    else {

        switch (key) {
            case 'f':
                bFullscreen = !bFullscreen;
                ofSetFullscreen(bFullscreen);
                break;
        }

    }




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

    ofPoint mousePoint;
    mousePoint.set(x, y);
    if (USB_Address->clickedIn(mousePoint)) {
        USB_Address->activate();
        networkAddress->deactivate();
    }
    else if (networkAddress->clickedIn(mousePoint)) {
        networkAddress->activate();
        USB_Address->deactivate();
    }
    else {
        USB_Address->deactivate();
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

    handTracker.trackBlobs(finder.blobs);

    for (int i = 0; i < handTracker.blobs.size(); i++) {

        maxFrameNum = (handTracker.blobs[i].age > maxFrameNum) ? handTracker.blobs[i].age : maxFrameNum;
        if (handTracker.blobs[i].age >= 30 && !highFiveSent) {
            giveHighFive();
            sendHighFiveMessage();
            highFiveSent = true;

        }

    }

}

bool testApp::notFaceCheck(ofxCvGrayscaleImage theImage) {

    faceFinder.findHaarObjects(theImage);

    for (int i = 0; i < faceFinder.blobs.size(); i++) {

        ofRectangle faceBorder = faceFinder.blobs[i].boundingRect;

    }

}

void testApp::blobDisappeared(int &_age) {

    if (_age >= 30) {

        if (highFiveSent) {
            endHighFive();
            highFiveSent = false;
            maxFrameNum = 0;
        }

    }

}

void testApp::giveHighFive() {

    string initString = "inithighfive";
    initString += "\n";


    ofLog(OF_LOG_WARNING, "sending high five initialization");
    for (int i = 0; i < initString.length(); i++) {
        handConnected = serial.writeByte(initString[i]);
    }

}

void testApp::endHighFive() {

    string endString = "endhighfive";
    endString += "\n";

    ofLog(OF_LOG_WARNING, "sending high five termination");
    for (int i = 0; i < endString.length(); i++) {
        handConnected = serial.writeByte(endString[i]);
    }

}

void testApp::zeroHand() {

    string zeroString = "returnzero";
    zeroString += "\n";
    for (int i = 0; i < zeroString.length(); i++) {
        serial.writeByte(zeroString[i]);
    }

}

void testApp::sendHighFiveMessage() {

    ofxOscMessage m;
    m.setAddress("/notification");
    m.addStringArg("incominghighfive");
    sender.sendMessage(m);

}

void testApp::checkForOscMessage() {

    while (receiver.hasWaitingMessages()) {

        ofxOscMessage m;
        string theMessage;
        receiver.getNextMessage(&m);

        if (m.getAddress() == "/notification") {
            theMessage = m.getArgAsString(0);
        }

        if (theMessage == "incominghighfive") {
            incomingHighFiveProtocol();
        }
    }

}

void testApp::incomingHighFiveProtocol() {

    ofSystemAlertDialog("INCOMING HIGH FIVE!!!");
    giveHighFive();
    ofSleepMillis(2000);
    endHighFive();

}

void testApp::exit() {

    zeroHand();

}
