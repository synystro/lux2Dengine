#pragma once

#include <vector>
#include <algorithm>
#include "./Controls.h"

class InputManager {
    private:
        std::vector<int> keysPressed;
    public:
        bool CheckKeyPressed(int key) {
            if(std::find(keysPressed.begin(), keysPressed.end(), key) != keysPressed.end()) {
                return true;
            }           
            keysPressed.push_back(key); 
            return false;
        }
        void SetKeyReleased(int key) {
            if(std::find(keysPressed.begin(), keysPressed.end(), key) != keysPressed.end()) {
                keysPressed.erase(std::remove(keysPressed.begin(), keysPressed.end(), key), keysPressed.end());
            }
        }
};