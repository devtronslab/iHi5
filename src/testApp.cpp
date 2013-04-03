#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    camWidth    = 320;
    camHeight   = 240;

    bFullscreen = false;

    searchRes   = 5;
    handsFound  = 0;
    handConnected = false;

//    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);

    colorImage.allocate(camWidth, camHeight);
    grayImage.allocate(camWidth, camHeight);

    ofBackground(0);
//    ofSetLogLevel(OF_LOG_VERBOSE);

    font.loadFont("DIN.otf", 16);

    finder.setScaleHaar(1.9);
    finder.setup("Hand.Cascade.1.xml");
    faceFinder.setScaleHaar(1.7);
    faceFinder.setup("haarcascade_frontalface_default.xml");
    maxFrameNum = 0;

    USB_Address = new textField(300, ofGetWindowHeight() - 91, 300, "/dev/ttyACM0");
    networkAddress = new textField(300, ofGetWindowHeight() - 116, 300, "128.192.4.254");

    devices = serial.getDeviceList();
    if (serial.setup(USB_Address->fieldString, 9600)) {
        zeroHand();
        handConnected = true;
    }

    sender.setup(networkAddress->fieldString, PORT);
    receiver.setup(PORT);

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
    ofPopMatrix();

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
            case ',':
                searchRes--;
                break;
            case '.':
                searchRes++;
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

    for (int i = 0; i < finder.blobs.size(); i++) {

        for (int j = 0; j < prevHaarBlobs.size(); j++) {

            if (finder.blobs[i].centroid.x > prevHaarBlobs[i].theCenter.x - searchRes &&
                    finder.blobs[i].centroid.x < prevHaarBlobs[i].theCenter.x + searchRes &&
                    finder.blobs[i].centroid.y > prevHaarBlobs[i].theCenter.y - searchRes &&
                    finder.blobs[i].centroid.y < prevHaarBlobs[i].theCenter.y + searchRes) {

                prevHaarBlobs[j].bBlobFound = true;
                break;

            }

        }

        prevHaarBlobs.push_back(previousBlobs(finder.blobs[i].centroid));


    }

    for (prevBlobIt = prevHaarBlobs.begin(); prevBlobIt != prevHaarBlobs.end(); prevBlobIt++) {

        if ((*prevBlobIt).bBlobFound) {

            (*prevBlobIt).bBlobFound = false;
            (*prevBlobIt).numFrames++;
//            ofLog(OF_LOG_NOTICE, "found reoccuring blob");
            maxFrameNum = ((*prevBlobIt).numFrames > maxFrameNum) ? (*prevBlobIt).numFrames : maxFrameNum;

            if ((*prevBlobIt).numFrames >= 5) {
                if (notFaceCheck(grayImage, *prevBlobIt)) {
                    (*prevBlobIt).numFrames = 0;
                    giveHighFive();
                    sendHighFiveMessage();
                    maxFrameNum = 0;
                }
                else {
                    (*prevBlobIt).numFrames = 0;
                    ofLog(OF_LOG_NOTICE, "face found");
                }
            }

        }
        else {

            prevHaarBlobs.erase(prevBlobIt);
//            ofLog(OF_LOG_NOTICE, "erasing blob");
            prevBlobIt--;   //erase command resizes vector, iterator rewind needed to prevent seg faults

        }

    }

}

bool testApp::notFaceCheck(ofxCvGrayscaleImage theImage, previousBlobs handCandidate) {

    faceFinder.findHaarObjects(theImage);

    for (int i = 0; i < faceFinder.blobs.size(); i++) {

        ofRectangle faceBorder = faceFinder.blobs[i].boundingRect;

        if (handCandidate.theCenter.x > faceFinder.blobs[i].centroid.x - faceBorder.width/2 &&
                handCandidate.theCenter.x < faceFinder.blobs[i].centroid.x + faceBorder.width/2 &&
                handCandidate.theCenter.y > faceFinder.blobs[i].centroid.y - faceBorder.height/2 &&
                handCandidate.theCenter.y < faceFinder.blobs[i].centroid.y + faceBorder.height/2) {

            return false;

        }
        else {
            return true;

        }
    }

}

void testApp::giveHighFive() {

    string initString = "inithighfive";
    initString += "\n";
    string endString = "endhighfive";
    endString += "\n";

    ofLog(OF_LOG_WARNING, "sending high five initialization");
    for (int i = 0; i < initString.length(); i++) {
        handConnected = serial.writeByte(initString[i]);
    }

    ofSleepMillis(5000);

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

void testApp::exit() {

    zeroHand();

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

}
