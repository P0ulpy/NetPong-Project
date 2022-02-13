#include "Engine/Engine.hpp"

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