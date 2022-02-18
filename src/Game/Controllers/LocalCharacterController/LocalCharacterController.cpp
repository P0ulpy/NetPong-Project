//
// Created by Flo on 11/02/2022.
//

#include "LocalCharacterController.hpp"
#include "../../Entities/Character.hpp"
#include "../../Entities/PongBall.hpp"
#include "../../../Engine/Scenes/MainGameScene.hpp"

LocalCharacterController::KeyMap::KeyMap(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left,
                                         sf::Keyboard::Key right, sf::Mouse::Button shoot)
        : up(up), down(down), left(left), right(right), shoot(shoot) {}


LocalCharacterController::LocalCharacterController(SyncableObjectOptions options, Character &character, const KeyMap &keymap)
    : ControllerBase(options, character)
    , _keyMap(keymap)
{

}

void LocalCharacterController::update(const float& deltaTime)
{
    auto& character = static_cast<Character &>(_controlTarget);

    if(character.canCharacterMove())
    {
        translate(deltaTime);
    }

    rotate();

    if (character.canCharacterShoot() && sf::Mouse::isButtonPressed(_keyMap.shoot))
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

    static_cast<Character &>(_controlTarget).moveEntity(sf::Vector2f(x, y), deltaTime);
}

void LocalCharacterController::shoot()
{
    if(!MainGameScene::getInstance()) return;

    auto& character = static_cast<Character &>(_controlTarget);
    auto& engine = PoPossibEngin::getInstance();
    auto inactivePongBalls = MainGameScene::getInstance()->getInactivePongBalls();

    if (!character.canCharacterMove() || !character.canCharacterShoot()) return;
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

sf::Packet LocalCharacterController::sync(std::stringstream &debugStream)
{
    std::lock_guard guard(_mutex);

    sf::Packet packet;
    PlayerState state = getCurrentPlayerState();

    packet << state.position.x << state.position.y << state.velocity.x << state.velocity.y << state.angle << state.angularVelocity;
    debugStream << "pos: {x:" << state.position.x << ", y:" << state.position.y << "}, vel: {x:" << state.velocity.x << ", y:" << state.velocity.y << "}, angle: " << state.angle << ", angleVel: " << state.angularVelocity;

    return packet;
}

void LocalCharacterController::applySync(sf::Packet &recievedPacketChunk, std::stringstream &debugStream)
{
    std::lock_guard guard(_mutex);

    int px, py;
    float vx, vy;
    float angle, angleVel;

    recievedPacketChunk >> px;
    recievedPacketChunk >> py;
    recievedPacketChunk >> vx;
    recievedPacketChunk >> vy;
    recievedPacketChunk >> angle;
    recievedPacketChunk >> angleVel;

    PlayerState state {
            {px, py},
            {vx, vy},
            angle,
            angleVel
    };

    debugStream << "pos: {x:" << state.position.x << ", y:" << state.position.y << "}, vel: {x:" << state.velocity.x << ", y:" << state.velocity.y << "}, angle: " << state.angle << ", angleVel: " << state.angularVelocity;
}


PlayerState LocalCharacterController::getCurrentPlayerState() const
{
    return {
            (sf::Vector2i)_controlTarget.getPosition(),
            _controlTarget.getVelocity(),
            _controlTarget.getRotation(),
            0
    };
}
