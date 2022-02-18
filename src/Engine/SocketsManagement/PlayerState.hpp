//
// Created by Flo on 17/02/2022.
//

#ifndef NETPONG_PROJECT_PLAYERSTATE_HPP
#define NETPONG_PROJECT_PLAYERSTATE_HPP

#include "SFML/System/Vector2.hpp"

struct PlayerState
{
    PlayerState() = default;
    PlayerState(const sf::Vector2i& position, const sf::Vector2f &velocity, float angle, float angularVelocity);

    sf::Vector2i position;
    sf::Vector2f velocity;

    float angle = 0;
    float angularVelocity = 0;
};

#endif //NETPONG_PROJECT_PLAYERSTATE_HPP
