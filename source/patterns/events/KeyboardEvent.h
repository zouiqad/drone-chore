
#ifndef KEYBOARDEVENT_H
#define KEYBOARDEVENT_H

#include "Event.h"

namespace n2m {
enum class KeyAction { PRESS, RELEASE, HOLD };

class KeyboardEvent : public Event {
public:
    KeyboardEvent (const int& keyCode,
        const KeyAction& action,
        const float& deltaTime)
        : keyCode (keyCode), action (action), deltaTime (deltaTime) {
    }

    int getKeyCode () const { return keyCode; }
    KeyAction getAction () const { return action; }
    float getDeltaTime () const { return deltaTime; }

private:
    int keyCode;
    KeyAction action;
    float deltaTime;
};
}

#endif //KEYBOARDEVENT_H
