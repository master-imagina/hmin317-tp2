#include "inputmanager.h"

map<int,bool> InputManager::keys;
map<int,float> InputManager::keys_smooth;
map<int,bool> InputManager::mouseButtons;
int InputManager::smooth_time_ms = 1000;
int InputManager::verticalWheel = 0;
int InputManager::horizontalWheel = 0;

InputManager::InputManager()
{

}

void InputManager::setKeyDown(int key){

    keys[key] = true;
}

void InputManager::setKeyUp(int key){

    keys[key] = false;
}

void InputManager::setButtonDown(int bt){

    mouseButtons[bt] = true;
}

void InputManager::setButtonUp(int bt){

    mouseButtons[bt] = false;
}

void InputManager::setWheel(int w, bool d){
    if(d){
        horizontalWheel = w;
    } else {
        verticalWheel = w;
    }
}

bool InputManager::isKeyDown(int key){

    if(existKeys(key))
        return keys[key];
    return false;
}

float InputManager::isKeyDown_Smooth(int key){

    if(existKeys_Smooth(key))
        return keys_smooth[key];
    return 0.0;
}

bool InputManager::isKeyPressed(int key){

    bool pressed = false;

    if(existKeys(key))
        pressed = keys[key];

    keys[key] = false;

    return pressed;
}

bool InputManager::isButtonDown(int bt){

    if(existButton(bt))
        return mouseButtons[bt];
    return false;
}

bool InputManager::isButtonPressed(int bt){

    bool pressed = false;

    if(existButton(bt))
        pressed = mouseButtons[bt];

    mouseButtons[bt] = false;

    return pressed;
}

int InputManager::getVariationBetweenKeys(int key1, int key2){
    return ((isKeyDown(key1))?1:0) + ((isKeyDown(key2))?-1:0);
}

float InputManager::getVariationBetweenKeys_Smooth(int key1, int key2){
    return isKeyDown_Smooth(key1) - isKeyDown_Smooth(key2);
}

int InputManager::getHorizontalWheel(){
    int h = horizontalWheel;
    horizontalWheel = 0;
    return h;
}

int InputManager::getVerticalWheel(){
    int v = verticalWheel;
    verticalWheel = 0;
    return v;
}

bool InputManager::existKeys(int key){
    return keys.find(key) != keys.end();
}

bool InputManager::existKeys_Smooth(int key){
    return keys_smooth.find(key) != keys_smooth.end();
}

bool InputManager::existButton(int bt){
    return mouseButtons.find(bt) != mouseButtons.end();
}

void InputManager::Update(int ms){

    float time = (float)ms / (float)smooth_time_ms;

    std::map<int, bool>::iterator it = keys.begin();
    while(it != keys.end())
    {
        int key = it->first;
        bool value = it->second;
        it++;

        if(existKeys_Smooth(key)){
            if(value){
                if(keys_smooth[key] < 1.0)
                    keys_smooth[key] += time;
                else
                    keys_smooth[key] = 1.0;
            } else {
                if(keys_smooth[key] > 0.0)
                    keys_smooth[key] -= time;
                else
                    keys_smooth[key] = 0.0;
            }
        } else if(value){
            keys_smooth[key] = time;
        }
    }
}
