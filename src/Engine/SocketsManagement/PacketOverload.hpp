//
// Created by flo on 11/02/2022.
//

#pragma once

#include <SFML/Network/Packet.hpp>
#include "Client/PlayerSettings.hpp"

namespace sf
{
    sf::Packet& operator <<(sf::Packet& packet, const PlayerSettings& playerSettings);
    sf::Packet& operator >>(sf::Packet& packet, PlayerSettings& playerSettings);
}