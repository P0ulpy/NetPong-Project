//
// Created by Flo on 11/02/2022.
//

#include "LocalCharacterController.hpp"
#include "../../Entities/Character.hpp"
#include "../../Entities/PongBall.hpp"
#include "../../../Engine/Scenes/MainGameScene.hpp"
#include "../NetworkCharacterController/NetworkCharacterController.hpp"

LocalCharacterController::KeyMap::KeyMap(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left,
                                         sf::Keyboard::Key right, sf::Mouse::Button shoot)
        : up(up), down(down), left(left), right(right), shoot(shoot) {}


LocalCharacterController::LocalCharacterController(Client::SyncableObjectOptions options, Character &character, const KeyMap &keymap)
    : SyncableObject(options)
    , ControllerBase(character)
    , _keyMap(keymap)
{

}

void LocalCharacterController::update(const float& deltaTime)
{
    rotate();
    translate(deltaTime);

    if (sf::Mouse::isButtonPressed(_keyMap.shoot))
        shoot();
}

float LocalCharacterController::calcRotFromMousePos(sf::Vector2i mousePos)
{
    sf::Vector2f curPos = _controlTarget.getPosition();
    float dx = curPos.x - (float)mousePos.x;
    float dy = curPos.y - (float)mousePos.y;
    return (float)((atan2(dy, dx)) * (float)180.0 / 3.14);
}

void LocalCharacterController::rotate()
{
    auto mousePosition = PoPossibEngin::getInstance().getInputsManager().getMousePosition();
    auto rotation = calcRotFromMousePos(mousePosition);
    _controlTarget.setRotation(rotation);
}

void LocalCharacterController::translate(const float& deltaTime)
{
    float x = 0;
    float y = 0;

    if(sf::Keyboard::isKeyPressed(_keyMap.right)) x = 1;
    if(sf::Keyboard::isKeyPressed(_keyMap.left)) x = -1;
    if (sf::Keyboard::isKeyPressed(_keyMap.down)) y = 1;
    if (sf::Keyboard::isKeyPressed(_keyMap.up)) y = -1;

    dynamic_cast<Character &>(_controlTarget).moveEntity(sf::Vector2f(x, y), deltaTime);
}

void LocalCharacterController::shoot()
{
    if(!MainGameScene::getInstance()) return;

    auto& character = dynamic_cast<Character &>(_controlTarget);
    auto& engine = PoPossibEngin::getInstance();
    auto inactivePongBalls = MainGameScene::getInstance()->getInactivePongBalls();

    if (inactivePongBalls.empty()) return;

    if (!character.isInCooldown() && !character.isReloading())
    {
        inactivePongBalls.top()->shoot(
                character.shootDepart(),
                character.shootDirection(engine.getInputsManager().getMousePosition()),
                character.getNormalAmmoColor(),
                character.getInactiveAmmoColor()
        );

        character.ammoCount(-1);
        character.activateCooldown(true);

        inactivePongBalls.pop();
    }
}

PlayerState LocalCharacterController::getCurrentPlayerState() const
{
    return {
            (sf::Vector2i)_controlTarget.getPosition(),
            _controlTarget.getVelocity(),
            _controlTarget.getRotation(),
            //TEMP
            0
    };
}
