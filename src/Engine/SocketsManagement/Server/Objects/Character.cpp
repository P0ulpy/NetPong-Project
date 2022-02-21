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
    int px, py;
    float vx, vy;
    float angle, angleVel;

    packet >> px;
    packet >> py;
    packet >> vx;
    packet >> vy;
    packet >> angle;
    packet >> angleVel;

    state = {
            {px, py},
            {vx, vy},
            angle, angleVel
    };

    //debugStream << "pos: {x:" << px << ", y:" << py << "}, vel: {x:" << vx << ", y:" << vy << "}, angle: " << angle << ", angleVel: " << angleVel;
}

