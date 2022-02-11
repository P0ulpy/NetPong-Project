//
// Created by flo on 11/02/2022.
//

#include "PacketOverload.hpp"

#include <SFML/Network/Packet.hpp>

sf::Packet& operator <<(sf::Packet& packet, const PlayerSettings& playerSettings)
{
    return packet << playerSettings.name << playerSettings.color;
}

sf::Packet& operator >>(sf::Packet& packet, PlayerSettings& playerSettings)
{
    return packet >> playerSettings.name >> playerSettings.color;
}