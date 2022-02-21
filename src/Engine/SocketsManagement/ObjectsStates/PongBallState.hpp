//
// Created by Flo on 18/02/2022.
//

#ifndef NETPONG_PROJECT_PONGBALLSTATE_HPP
#define NETPONG_PROJECT_PONGBALLSTATE_HPP

#include "SFML/System/Vector2.hpp"
#include "../Server/Globals.hpp"

struct PongBallState
{
    PongBallState() = default;
    PongBallState(const sf::Vector2i& position, const sf::Vector2f &velocity, bool enabled, bool canKill);

    sf::Vector2i position { sf::Vector2i(MapWidth / 2, MapHeight / 2) };
    sf::Vector2f velocity { sf::Vector2i(0, 0) };

    bool enabled = false;
    bool canKill = false;
};


#endif //NETPONG_PROJECT_PONGBALLSTATE_HPP
