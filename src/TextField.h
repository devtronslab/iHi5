#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "ofMain.h"

class TextField
{
    public:
        TextField(int _x, int _y, int _width, string initString);
        virtual ~TextField();

        void update();
        void draw();

        bool clickedIn(ofPoint mousePos);
        void activate();
        void deactivate();
        bool isSelected();
        void addChar(char additionalChar);
        void delChar();

        string fieldString;
        ofTrueTypeFont  font;

        int x;
        int y;
        int width;
        int height;

    protected:

        ofPoint          cursorTop;
        int             stringWidth;
        bool            initialString;
        ofRectangle     theBox;
        bool            isActivated;
        bool            cursorSwitcher;
        int             cursorBlinker;

    private:
};

#endif // TEXTFIELD_H
