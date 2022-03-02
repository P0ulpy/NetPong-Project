//
// Created by Flo on 15/02/2022.
//

#ifndef NETPONG_PROJECT_ISYNCABLE_HPP
#define NETPONG_PROJECT_ISYNCABLE_HPP

#include <SFML/Network/Packet.hpp>

namespace Server
{
    class SyncableObject;

    class ISyncable
    {
    public:
        ISyncable() = default;
        virtual sf::Packet sync() = 0;
        virtual void applySync(sf::Packet& packet) = 0;

        SyncableObject* rootObject;
    };
}

#endif //NETPONG_PROJECT_ISYNCABLE_HPP
