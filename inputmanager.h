#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <map>

#include "global.h"

class InputManager
{
    public:
        InputManager();

        static void setKeyDown(int);
        static void setKeyUp(int);
        static void setButtonDown(int);
        static void setButtonUp(int);
        static void setWheel(int,bool);

        static bool isKeyDown(int);
        static float isKeyDown_Smooth(int);
        static bool isKeyPressed(int);

        static bool isButtonDown(int);
        static bool isButtonPressed(int);

        static int getVerticalWheel();
        static int getHorizontalWheel();

        static int getVariationBetweenKeys(int,int);
        static float getVariationBetweenKeys_Smooth(int,int);

        static void Update(int ms);

        static int smooth_time_ms;

    private:
        static map<int,bool> keys;
        static map<int,bool> mouseButtons;
        static map<int,float> keys_smooth;

        static int verticalWheel;
        static int horizontalWheel;

        static bool existKeys(int);
        static bool existKeys_Smooth(int);
        static bool existButton(int);
};

#endif // INPUTMANAGER_H
