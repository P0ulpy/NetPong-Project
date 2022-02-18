//
// Created by Flo on 15/02/2022.
//

#ifndef NETPONG_PROJECT_CHARACTER_HPP
#define NETPONG_PROJECT_CHARACTER_HPP

#include "ISyncable.hpp"
#include "../../PlayerState.hpp"

namespace Server
{
    class Character : public ISyncable
    {
    public:
        Character() = default;
        Character(PlayerState state);

        PlayerState state;
    };
}

#endif //NETPONG_PROJECT_CHARACTER_HPP
