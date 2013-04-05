#include "TextField.h"

TextField::TextField(int _x, int _y, int _width, string initString)
{

    font.loadFont("DIN.otf", 12);
    x = _x;
    y = _y;
    width = _width;
    height = font.stringHeight("Blah") * 2;

    theBox = ofRectangle(x, y, width, height);
    fieldString = initString;
    initialString = true;
    isActivated = false;
    cursorSwitcher = true;
    cursorBlinker = 0;

}

TextField::~TextField()
{
    //dtor
}

void TextField::update() {

    stringWidth = font.stringWidth(fieldString);
    cursorTop.set(x + 15 + stringWidth, y + height/4);
    if (!cursorBlinker)
        cursorSwitcher = !cursorSwitcher;
    cursorBlinker = (cursorBlinker + 1) % 30;
    theBox = ofRectangle(x, y, width, height);

}

void TextField::draw()  {

    ofPushMatrix();
        ofSetColor(255,255,255);
        ofNoFill();
        ofRect(theBox);
        font.drawString(fieldString, x + 15, y + height*3/4);
        if (isActivated && cursorSwitcher) {
            ofPoint cursorBottom;
            cursorBottom.set(cursorTop.x, cursorTop.y + height/2);
            ofLine(cursorTop, cursorBottom);
        }

    ofPopMatrix();

}

bool TextField::clickedIn(ofPoint mousePos) {

    return theBox.inside(mousePos);

}

void TextField::activate() {

    isActivated = true;

}

void TextField::deactivate() {

    isActivated = false;

}

bool TextField::isSelected() {

    return isActivated;

}

void TextField::addChar(char additionalChar){

    fieldString += additionalChar;

}

void TextField::delChar() {

    int stringIndex = fieldString.length();
    string tmpString = "";
    for (int i = 0; i < stringIndex - 1; i++) {
        tmpString += fieldString[i];
    }
    fieldString = tmpString;

}
