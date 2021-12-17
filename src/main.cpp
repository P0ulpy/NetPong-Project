#include "Engine/Engine.hpp"
#include "Utils/EventEmitter.hpp"

#include <iostream>
#include "Utils/NetTools.hpp"

EventEmitter eventEmitter;

int main(int argc, char** argv)
{
    PoPossibEngin engine = PoPossibEngin(
        EngineConfig(
            EngineConfig::WindowConfig(
                sf::VideoMode(900, 900),
                "Netpong"
            )
        )
    );

    engine.start();
}