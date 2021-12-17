#include "Engine/Engine.hpp"
#include "Utils/EventEmitter.hpp"

EventEmitter eventEmitter;

int main(int argc, char** argv)
{
    PoPossibEngin engine = PoPossibEngin(
        EngineConfig(
            EngineConfig::WindowConfig(
                sf::VideoMode(1200, 900),
                "Netpong"
            )
        )
    );

    engine.start();
}