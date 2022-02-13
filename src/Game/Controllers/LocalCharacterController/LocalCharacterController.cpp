//
// Created by Flo on 11/02/2022.
//

#include "LocalCharacterController.hpp"

LocalCharacterController::LocalCharacterController(Character &character)
        : _character(character) {}

void LocalCharacterController::Update(float dt)
{
    auto mousePosition = PoPossibEngin::getInstance().getInputsManager().getMousePosition();
    auto rotation = calcRotFromMousePos(mousePosition);
    _character.setRotation(rotation);
}

float LocalCharacterController::calcRotFromMousePos(sf::Vector2i mousePos)
{
    sf::Vector2f curPos = _character.getCanon().getPosition();
    float dx = curPos.x - (float)mousePos.x;
    float dy = curPos.y - (float)mousePos.y;
    return ((atan2(dy, dx)) * (float)180.0 / 3.14);
}
