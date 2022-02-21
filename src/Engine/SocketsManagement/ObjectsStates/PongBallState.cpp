//
// Created by Flo on 18/02/2022.
//

#include "PongBallState.hpp"

PongBallState::PongBallState(
        const sf::Vector2i &position,
        const sf::Vector2f &velocity,
        bool enabled,
        bool canKill)

        : velocity(velocity)
        , position(position)
        , enabled(enabled)
        , canKill(canKill)
{

}
