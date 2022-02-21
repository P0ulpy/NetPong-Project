//
// Created by Flo on 18/02/2022.
//

#ifndef NETPONG_PROJECT_NETWORKPONGBALLCONTROLLER_HPP
#define NETPONG_PROJECT_NETWORKPONGBALLCONTROLLER_HPP

#include "../ControllerBase.hpp"
#include "../../../Engine/SocketsManagement/ObjectsStates/PongBallState.hpp"
#include "../../Entities/PongBall.hpp"

class NetworkPongBallController : public Engine::ControllerBase
{
public:
    NetworkPongBallController(SyncableObjectOptions options, PongBall &pongBall, const PongBallState& pongBallState);

    sf::Packet sync(std::stringstream& debugStream) override;

    static PongBallState extractData(sf::Packet& recievedPacketChunk);
    void applySync(sf::Packet& recievedPacketChunk, std::stringstream& debugStream) override;
    void applySync(const PongBallState& state, std::stringstream& debugStream);
    void update(const float& deltaTime) override;

    PongBallState getPongBallState();

private:
    PongBall& _pongBall;
};


#endif //NETPONG_PROJECT_NETWORKPONGBALLCONTROLLER_HPP
