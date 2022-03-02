//
// Created by Flo on 17/02/2022.
//

#ifndef NETPONG_PROJECT_PLAYERSTATE_HPP
#define NETPONG_PROJECT_PLAYERSTATE_HPP

#include "SFML/System/Vector2.hpp"
#include "../Server/Globals.hpp"

struct PlayerState
{
    PlayerState() = default;
    PlayerState(const sf::Vector2i& position, const sf::Vector2f &velocity, float angle, float angularVelocity);

    sf::Vector2i position { sf::Vector2i(MapWidth / 2, MapHeight / 2) };
    sf::Vector2f velocity { sf::Vector2i(0, 0) };

    float angle = 0;
    float angularVelocity = 0;
    float radius { 18 };
};

#endif //NETPONG_PROJECT_PLAYERSTATE_HPP
