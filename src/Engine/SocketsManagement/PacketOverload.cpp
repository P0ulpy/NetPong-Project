//
// Created by flo on 11/02/2022.
//

#include "PacketOverload.hpp"

#include <SFML/Network/Packet.hpp>

/*sf::Packet& operator <<(sf::Packet& packet, const PlayerSettings& playerSettings)
{
    return packet << playerSettings.name << playerSettings.color;
}

sf::Packet& operator >>(sf::Packet& packet, PlayerSettings& playerSettings)
{
    return packet >> playerSettings.name >> playerSettings.color;
}

sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2i& vector) { return packet << vector.x << vector.y; }
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2i& vector) { return packet >> vector.x >> vector.y; }

sf::Packet& operator <<(sf::Packet& packet, const sf::Vector2<float>& vector) { return packet << vector.x << vector.y; }
sf::Packet& operator >>(sf::Packet& packet, sf::Vector2<float>& vector) { return packet >> vector.x >> vector.y; }

sf::Packet& operator <<(sf::Packet& packet, const PlayerState& playerState)
{
    return packet << playerState.position << playerState.angle << playerState.velocity << playerState.angularVelocity;
}

sf::Packet& operator >>(sf::Packet& packet, PlayerState& playerState)
{
    return packet >> playerState.position >> playerState.angle >> playerState.velocity >> playerState.angularVelocity;
}*/
