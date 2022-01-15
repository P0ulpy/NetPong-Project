#include "Engine/Engine.hpp"

int main(int argc, char** argv)
{
    PoPossibEngin engine(
        EngineConfig(
            EngineConfig::WindowConfig(
                sf::VideoMode(1200, 900),
                "Netpong"
            )
        )
    );

    engine.start();
}