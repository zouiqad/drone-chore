#include <iostream>
#include "core/Engine.h"


int main () {
    auto engine = std::make_shared<n2m::Engine> ();

    if (!engine->init ()) {
        std::cout << "Failed to initialize engine" << std::endl;
        return 1;
    }

    engine->run ();

    return 0;
}