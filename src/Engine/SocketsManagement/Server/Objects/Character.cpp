//
// Created by Flo on 15/02/2022.
//

#include "Character.hpp"

Server::Character::Character(PlayerState state)
        : ISyncable()
        , state(state)
{
}

