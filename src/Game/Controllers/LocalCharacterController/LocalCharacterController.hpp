//
// Created by Flo on 11/02/2022.
//

#ifndef NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP
#define NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP

#include "../../Entities/Character.hpp"

class Character;

class LocalCharacterController
{
private:
    Character& _character;

public:
    explicit LocalCharacterController(Character &character);
    void Update(float dt);

    float calcRotFromMousePos(sf::Vector2i mousePos);
};

#endif //NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP
