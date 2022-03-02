//
// Created by Flo on 15/02/2022.
//

#ifndef NETPONG_PROJECT_CHARACTER_HPP
#define NETPONG_PROJECT_CHARACTER_HPP

#include "ISyncable.hpp"
#include "../../ObjectsStates/PlayerState.hpp"
#include <SFML/Network/Packet.hpp>

namespace Server
{
    class Character : public ISyncable
    {
    public:
        Character() = default;
        explicit Character(PlayerState state);

        sf::Packet sync() override;
        void applySync(sf::Packet& packet) override;

        PlayerState state;
    };
}

#endif //NETPONG_PROJECT_CHARACTER_HPP
