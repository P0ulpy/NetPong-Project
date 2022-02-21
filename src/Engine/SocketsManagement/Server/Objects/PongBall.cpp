//
// Created by Flo on 18/02/2022.
//

#include "PongBall.hpp"

Server::PongBall::PongBall(PongBallState state)
        : ISyncable()
        , state(state)
{

}

sf::Packet Server::PongBall::sync()
{
    sf::Packet pongballStatePacket;

    pongballStatePacket << state.position.x;
    pongballStatePacket << state.position.y;
    pongballStatePacket << state.velocity.x;
    pongballStatePacket << state.velocity.y;
    pongballStatePacket << state.enabled;
    pongballStatePacket << state.canKill;

    return pongballStatePacket;
}

void Server::PongBall::applySync(sf::Packet &packet)
{
    packet >> state.position.x;
    packet >> state.position.y;
    packet >> state.velocity.x;
    packet >> state.velocity.y;
    packet >> state.enabled;
    packet >> state.canKill;
}
