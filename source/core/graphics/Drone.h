#ifndef DRONE_H
#define DRONE_H

#include <vector>
#include "Node.h"

namespace n2m::graphics {
    struct Waypoint {
        int x, y, z;
        uint32_t frame;
    };

    class Drone : public Node
    {
    private:
        std::vector<Waypoint> waypoints;
    public:
        Drone(std::shared_ptr<Model> model = nullptr):Node(model){};
        //~Drone();

        void updatePosition(uint32_t t)
        {
            if(t >= waypoints[waypoints.size() - 1].frame)
            {
                this->setTranslation({waypoints[waypoints.size() - 1].x / 100.0f, waypoints[waypoints.size() - 1].y / 100.0f, waypoints[waypoints.size() - 1].z / 100.0f});
                return;
            }
            int wp = 0;
            while (waypoints[++wp].frame <= t);
            int diff = waypoints[wp].frame - waypoints[wp - 1].frame;
            this->setTranslation((((t - waypoints[wp - 1].frame) / (float)diff) * glm::vec3{waypoints[wp].x / 100.0f, waypoints[wp].y / 100.0f, waypoints[wp].z / 100.0f}) + 
                                ((1.0f - ((t - waypoints[wp - 1].frame) / (float)diff)) * glm::vec3{waypoints[wp - 1].x / 100.0f, waypoints[wp - 1].y / 100.0f, waypoints[wp - 1].z / 100.0f}));
        
        }

        void addWaypoint(int _x, int _y, int _z, uint32_t _frame)
        {
            this->waypoints.push_back({_x, _y, _z, _frame});
        }

        static float distance(Drone d1, Drone d2)
        {
            return (d1.getTranslation() - d2.getTranslation()).length();
        }

        virtual std::string whoAmI(){return "Drone";}
    };
}

#endif