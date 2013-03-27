#include "testApp.h"

int fingerTipHits = 0;
int webbingHits = 0;

//--------------------------------------------------------------
void testApp::setup(){

    camWidth    = 720;
    camHeight   = 540;

    bFullscreen = false;
    bAdaptive   = true;

    threshold   = 200;
    blurLevel   = 3;
    blockSize   = 20;
    bGaussian   = false;
    bLearnBG    = false;

    handsFound  = 0;
    fingerTipThresh = 40;
    detectResolution= 5;

    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);

    colorImage.allocate(camWidth, camHeight);
    grayImage.allocate(camWidth, camHeight);
    backgroundImgAg.allocate(camWidth, camHeight);
    differenceImg.allocate(camWidth, camHeight);

    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);

    font.loadFont("DIN.otf", 20);

    serial.enumerateDevices();
    serial.setup("/dev/ttyACM0", 9600);

//    finder.setup("Hand.Cascade.1.xml");

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

        if (bLearnBG) {
//            if (backgroundImg.size() < 2) {
//                it = backgroundImg.begin();
//                backgroundImg.insert(it, grayImage);
//            }
//            else {
//                it = backgroundImg.begin();
//                backgroundImg.insert(it, grayImage);
//                backgroundImg.pop_back();
//            }

/*      code from a bLearnBG conditional below that I have pasted here in case I want to combine the two methods */

//            normalizer += 1/(backgroundImg.size()+1);
//            for (int i = 0; i < backgroundImg.size(); i++) {
//                tmpImage = backgroundImg[i];
//                tmpImage *= normalizer;
//                backgroundImgAg += tmpImage;
//                tmpImage.clear();
//            }
//            tmpImage = grayImage;
//            tmpImage *= normalizer;
            //backgroundImgAg *= normalizer;
//            differenceImg = backgroundImgAg;
//            differenceImg += tmpImage;
            //differenceImg.absDiff(backgroundImgAg, grayImage);
//            normalizer.clear();
//            tmpImage.clear();
//            backgroundImgAg = backgroundImg[0];
//            differenceImg.absDiff(backgroundImgAg, grayImage);

            backgroundImgAg = grayImage;
        }
        else {
            backgroundImgAg.set(0);
        }

        grayImage = colorImage;


//        differenceImg.absDiff(backgroundImgAg, grayImage);
        ofxCvGrayscaleImage tmpImage = grayImage;
        tmpImage -= backgroundImgAg;
        differenceImg = tmpImage;

        differenceImg.erode_3x3();
        differenceImg.dilate_3x3();

        if (bGaussian)
            differenceImg.blurGaussian(blurLevel);
        else
            differenceImg.blur(blurLevel);

        if (bAdaptive)
            differenceImg.adaptiveThreshold(threshold);
        else
            differenceImg.threshold(threshold);

        contourFinder.findContours(differenceImg, blockSize, (camWidth*camHeight)/3, 10, true, false);
        for (int i = 0; i < contourFinder.nBlobs; i++)
            smoothContour(contourFinder.blobs[i]);

    }

}

//--------------------------------------------------------------
void testApp::draw(){

    string msg;
    msg += keyToDisplay;
    font.drawString(msg, 900, 500);
    font.drawString(ofToString(backgroundImg.size()), 900, 450);

    font.drawString("threshold( [/] ): ", 100, (ofGetWindowHeight() - 50));
    font.drawString(ofToString(threshold), 350, (ofGetWindowHeight() - 50));
    font.drawString("blur level(up/down): ", 100, (ofGetWindowHeight() - 100));
    font.drawString(ofToString(blurLevel), 350, (ofGetWindowHeight() - 100));
    font.drawString("block size( </> ): ", 100, (ofGetWindowHeight() - 150));
    font.drawString(ofToString(blockSize), 350, (ofGetWindowHeight() - 150));
    font.drawString("background learn(i): ", 500, (ofGetWindowHeight() - 50));
    font.drawString(ofToString(bLearnBG), 750, (ofGetWindowHeight() - 50));
    font.drawString("frame rate:", 500, (ofGetWindowHeight() - 150));
    font.drawString(ofToString(ofGetFrameRate()), 750, (ofGetWindowHeight() - 150));

    font.drawString("fingerTipThresh:", 1000, (ofGetWindowHeight() - 100));
    font.drawString(ofToString(fingerTipThresh), 1250, (ofGetWindowHeight() - 100));
    font.drawString("webbingThresh:", 1000, (ofGetWindowHeight() - 150));
    font.drawString(ofToString(webbingThresh), 1250, (ofGetWindowHeight() - 150));
    font.drawString("detectResolution:", 1000, (ofGetWindowHeight() - 200));
    font.drawString(ofToString(detectResolution), 1250, (ofGetWindowHeight() - 200));


//    for (int i=0; i < contourFinder.nBlobs; i++) {
//        handDetectHaar(contourFinder.blobs[i]);
//        differenceImg.drawBlobIntoMe(contourFinder.blobs[i], 125);
//    }

    for (int i = 0; i < contourFinder.nBlobs; i++) {

        handDetect(contourFinder.blobs[i]);
        differenceImg.drawBlobIntoMe(contourFinder.blobs[i], 125);

    }

    differenceImg.draw(20,20);
    backgroundImgAg.draw(camWidth + 40, 20);

    font.drawString("number of blobs: ", 500, (ofGetWindowHeight() - 100));
    font.drawString(ofToString(contourFinder.nBlobs), 750, (ofGetWindowHeight() - 100));
    font.drawString("number of hands found:", 1000, (ofGetWindowHeight() - 50));
    font.drawString(ofToString(handsFound), 1400, (ofGetWindowHeight() - 50));
    font.drawString(ofToString(fingerTipHits), 1000, (ofGetWindowHeight() - 250));
    font.drawString(ofToString(webbingHits), 1000, (ofGetWindowHeight() - 300));

    ofSetColor(255,255,255);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    keyToDisplay = ofToString(key);

    switch (key) {
        case 357:
            blurLevel++;
            break;
        case 359:
            blurLevel--;
            break;
        case 91:
            threshold--;
            break;
        case 93:
            threshold++;
            break;
        case 46:
            blockSize++;
            break;
        case 44:
            blockSize--;
            break;
        case 'g':
            bGaussian = !bGaussian;
            break;
        case 'i':
            bLearnBG = !bLearnBG;
            if (!bLearnBG)
                backgroundImg.clear();
            break;
        case 'f':
            bFullscreen = !bFullscreen;
            ofSetFullscreen(bFullscreen);
            break;
        case 'a':
            bAdaptive = !bAdaptive;
            break;
        case 'q':
            fingerTipThresh--;
            break;
        case 'w':
            fingerTipThresh++;
            break;
        case 'o':
            webbingThresh--;
            break;
        case 'p':
            webbingThresh++;
            break;
        case '-':
            detectResolution -= 5;
            break;
        case '=':
            detectResolution += 5;
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

void testApp::handDetect(ofxCvBlob contour){

    float maxAngle = 0;

    for (int i = 0; i < contour.pts.size(); i += detectResolution/2) {

        ofVec2f v0;
        v0.x = contour.pts[i].x;
        v0.y = contour.pts[i].y;

        ofVec2f v1;
        v1.x = contour.pts[(i+detectResolution)%contour.pts.size()].x;
        v1.y = contour.pts[(i+detectResolution)%contour.pts.size()].y;

        ofVec2f v2;
        v2.x = contour.pts[(i-detectResolution)%contour.pts.size()].x;
        v2.y = contour.pts[(i-detectResolution)%contour.pts.size()].y;

        float angle1 = v1.angle(v0);
        float angle2 = v2.angle(v0);

        float angleDiff = angle2 - angle1;

        if (angleDiff > maxAngle)
            maxAngle = angleDiff;

        if (angleDiff > fingerTipThresh)
            fingerTipHits++;
//        if (angleDiff < 15)
//            webbingHits++;
//        else if (angleDiff > 345)
//            fingerTipHits++;

    }

    ofLog(OF_LOG_NOTICE, ofToString(detectResolution));
    ofLog(OF_LOG_NOTICE, ofToString(fingerTipHits));
    ofLog(OF_LOG_NOTICE, ofToString(maxAngle));

//    if (fingerTipHits >= 5 && webbingHits >= 4)
//        handsFound++;
    if (fingerTipHits >= 5)
        handsFound++;

}

void testApp::handDetectHaar(ofxCvBlob contourDealy) {

    int cornerx = 0;
    int cornery = 0;
    int width = 0;
    int height = 0;

    for (int i = 0; i < contourDealy.pts.size(); i++) {

        if  (contourDealy.pts[i].x < cornerx)
            cornerx = contourDealy.pts[i].x;
        if  (contourDealy.pts[i].y < cornery)
            cornery = contourDealy.pts[i].y;

        for (int j = 1; j < contourDealy.pts.size(); j += 20) {

            int xDiff = abs(contourDealy.pts[i].x - contourDealy.pts[j%contourDealy.pts.size()].x);
            int yDiff = abs(contourDealy.pts[i].y - contourDealy.pts[j%contourDealy.pts.size()].y);

            if (width < xDiff)
                width = xDiff;
            if (height < yDiff)
                height = yDiff;


        }

    }

    differenceImg.setROI(cornerx, cornery, width, height);
    finder.findHaarObjects(differenceImg);
    differenceImg.resetROI();
}

void testApp::smoothContour(ofxCvBlob blobby) {

    for (int i = 0; i < blobby.nPts; i++) {

        int lastPoint = (i-1)%blobby.nPts;
        int nextPoint = (i+1)%blobby.nPts;

        blobby.pts[i] = (blobby.pts[lastPoint] + blobby.pts[i] + blobby.pts[nextPoint])/3;

    }

}
