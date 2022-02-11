#include <iostream>
#include "Engine/Engine.hpp"
#include "Logger/Logger.hpp"

int main(int argc, char** argv)
{
    PoPossibEngin engine(
        EngineConfig(
            EngineConfig::WindowConfig(
                sf::VideoMode(500, 500),
                "Netpong"
            )
        )
    );

    engine.start();
}