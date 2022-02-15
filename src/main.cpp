#include "Engine/Engine.hpp"
#include "Logger/Logger.hpp"

int main(int argc, char** argv)
{
    PoPossibEngin engine(
        EngineConfig(
            EngineConfig::WindowConfig(
                sf::VideoMode(1920, 1080),
                "Netpong",
                sf::Style::Fullscreen
            )
        )
    );

    engine.start();
}
