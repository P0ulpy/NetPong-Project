#include "Engine/Engine.hpp"

int main(int argc, char** argv)
{
    PoPossibEngin engine(
    EngineConfig{
        EngineConfig::WindowConfig{
        sf::VideoMode(1920, 1080),
        "Net-pong"
        }
    });

    engine.start();
}