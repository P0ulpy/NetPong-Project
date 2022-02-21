//
// Created by Flo on 18/02/2022.
//

#ifndef NETPONG_PROJECT_PONGBALL_HPP
#define NETPONG_PROJECT_PONGBALL_HPP

#include "ISyncable.hpp"
#include "../../ObjectsStates/PongBallState.hpp"

namespace Server
{
    class PongBall : public ISyncable
    {
    public:
        PongBall() = default;
        PongBall(PongBallState state);

        sf::Packet sync() override;
        void applySync(sf::Packet& packet) override;

        PongBallState state;
    };
}


#endif //NETPONG_PROJECT_PONGBALL_HPP
