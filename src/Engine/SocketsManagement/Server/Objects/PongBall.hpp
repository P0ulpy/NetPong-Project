//
// Created by Flo on 18/02/2022.
//

#ifndef NETPONG_PROJECT_PONGBALL_HPP
#define NETPONG_PROJECT_PONGBALL_HPP

#include "ISyncable.hpp"
#include "../../ObjectsStates/PongBallState.hpp"
#include "SFML/Graphics/Color.hpp"
#include "../SyncManagement/SyncableObject.hpp"

namespace Server
{
    class PongBall : public ISyncable
    {
    public:
        PongBall() = default;
        explicit PongBall(PongBallState state);

        sf::Packet sync() override;
        void applySync(sf::Packet& packet) override;
        [[nodiscard]] bool hitPlayer(float c2x, float c2y, float c2r, const std::string& ownerID) const;

        PongBallState state;
    };
}


#endif //NETPONG_PROJECT_PONGBALL_HPP
