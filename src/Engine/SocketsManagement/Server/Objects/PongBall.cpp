//
// Created by Flo on 18/02/2022.
//

#include "PongBall.hpp"
#include "../../../../Utils/Utils.hpp"
#include "../../../../Logger/Logger.hpp"

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

bool Server::PongBall::hitPlayer(float c2x, float c2y, float c2r, const std::string& ownerID) const
{
    std::stringstream debugStream;
    debugStream
        << "\n{ pos: {x:" << state.position.x << ", y:" << state.position.y
        << "}, vel: {x:" << state.velocity.x << ", y:" << state.velocity.y
        << "}, radius:" << state.velocity.x
        << ", enabled: " << state.enabled << ", canKill: " << state.canKill
        << " }\n";

    debugStream
        << "{ pos: {x:" << c2x << ", y:" << c2y << "}, "
        << "radius:" << c2r;

    Logger::Log(debugStream.str());

    if (state.canKill && state.enabled && ownerID != rootObject->getController().getId())
    {
        return Utils::circleCircleCollision(
            (float)state.position.x,
            (float)state.position.y,
            state.radius, c2x, c2y, c2r
        );
    }

    return false;
}
