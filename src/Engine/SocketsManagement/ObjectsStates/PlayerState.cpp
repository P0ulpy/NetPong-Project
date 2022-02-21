//
// Created by Flo on 17/02/2022.
//

#include "PlayerState.hpp"

PlayerState::PlayerState(const sf::Vector2i& position, const sf::Vector2f &velocity, float angle, float angularVelocity)
        : velocity(velocity), position(position), angle(angle), angularVelocity(angularVelocity) {}
