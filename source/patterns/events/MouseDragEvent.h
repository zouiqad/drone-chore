#ifndef MOUSEDRAGEVENT_H
#define MOUSEDRAGEVENT_H
#include "Event.h"

namespace n2m {
class MouseDragEvent : public Event {
public:
    MouseDragEvent(const float &x, const float &y) : xoffset(x), yoffset(y) {
    };

    double xoffset, yoffset;
};
}

#endif //MOUSEDRAGEVENT_H
