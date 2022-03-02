//
// Created by Flo on 15/02/2022.
//

#include "Character.hpp"

Server::Character::Character(PlayerState state)
        : ISyncable()
        , state(state)
{

}

sf::Packet Server::Character::sync()
{
    sf::Packet packet;

    packet
        << state.position.x << state.position.y
        << state.velocity.x << state.velocity.y
        << state.angle << state.angularVelocity;

    return packet;
}

void Server::Character::applySync(sf::Packet& packet)
{
    packet >> state.position.x;
    packet >> state.position.y;
    packet >> state.velocity.x;
    packet >> state.velocity.y;
    packet >> state.angle;
    packet >> state.angularVelocity;

    //debugStream << "pos: {x:" << px << ", y:" << py << "}, vel: {x:" << vx << ", y:" << vy << "}, angle: " << angle << ", angleVel: " << angleVel;
}

