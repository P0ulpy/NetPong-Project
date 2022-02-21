//
// Created by Flo on 11/02/2022.
//

#include "LocalCharacterController.hpp"

#include <utility>
#include "../../Entities/Character.hpp"
#include "../../Entities/PongBall.hpp"
#include "../../../Engine/Scenes/MainGameScene.hpp"
#include "../../Terrains/PolygonTerrain.hpp"
#include "../../../Logger/Logger.hpp"

LocalCharacterController::KeyMap::KeyMap(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left,
                                         sf::Keyboard::Key right, sf::Mouse::Button shoot)
        : up(up), down(down), left(left), right(right), shoot(shoot) {}


LocalCharacterController::LocalCharacterController(SyncableObjectOptions options, Character &character, const KeyMap &keymap, const PlayerState& playerState)
    : ControllerBase(std::move(options), character)
    , _keyMap(keymap)
    , _character(character)
{
    std::stringstream debugStream;
    applySync(playerState, debugStream);
}

void LocalCharacterController::update(const float& deltaTime)
{
    if(_character.canCharacterMove())
    {
        translate(deltaTime);
    }

    rotate();

    if (_character.canCharacterShoot() && sf::Mouse::isButtonPressed(_keyMap.shoot))
        shoot();
}

float LocalCharacterController::calcRotFromMousePos(sf::Vector2i mousePos)
{
    sf::Vector2i curPos = _controlTarget.getPosition();
    int dx = curPos.x - mousePos.x;
    int dy = curPos.y - mousePos.y;
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

    _character.moveEntity(sf::Vector2f(x, y), deltaTime);
}

PongBall* LocalCharacterController::shoot()
{
    if(!MainGameScene::getInstance()) return nullptr;

    if (!_character.canCharacterMove() || !_character.canCharacterShoot()) return nullptr;
    if (_character.isInCooldown() || _character.isReloading()) return nullptr;

    Logger::Log("Shooting");

    auto& engine = PoPossibEngin::getInstance();
    auto* pongBall = _character.getOneInactivePongball();

    if(!pongBall)
    {
        Logger::Err("Can't shoot, no available pongBall");
        return nullptr;
    }

    pongBall->shoot(
            _character.shootDepart(),
            _character.shootDirection(engine.getInputsManager().getMousePosition()),
            _character.getNormalAmmoColor(),
            _character.getInactiveAmmoColor()
    );

    _character.ammoCount(-1);
    _character.activateCooldown(true);

    return pongBall;
}

sf::Packet LocalCharacterController::sync(std::stringstream &debugStream)
{
    std::lock_guard guard(_mutex);

    sf::Packet packet;
    PlayerState state = getCurrentPlayerState();

    packet << state.position.x << state.position.y << state.velocity.x << state.velocity.y << state.angle << state.angularVelocity;

    debugStream
        << "pos: {x:" << state.position.x << ", y:"
        << state.position.y << "}, vel: {x:" << state.velocity.x << ", y:" << state.velocity.y
        << "}, angle: " << state.angle << ", angleVel: " << state.angularVelocity;

    return packet;
}

void LocalCharacterController::applySync(sf::Packet &recievedPacketChunk, std::stringstream &debugStream)
{
    PlayerState state = LocalCharacterController::extractData(recievedPacketChunk);
    applySync(state, debugStream);
}
void LocalCharacterController::applySync(const PlayerState& state, std::stringstream& debugStream)
{
    Logger::Log("applying sync");

    _controlTarget.setPosition(state.position);
    _controlTarget.setVelocity(state.velocity);
    _controlTarget.setRotation(state.angle);

    debugStream
        << "pos: {x:" << state.position.x << ", y:"
        << state.position.y << "}, vel: {x:" << state.velocity.x << ", y:" << state.velocity.y
        << "}, angle: " << state.angle << ", angleVel: " << state.angularVelocity;
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

PlayerState LocalCharacterController::extractData(sf::Packet &recievedPacketChunk)
{
    PlayerState state;

    recievedPacketChunk >> state.position.x;
    recievedPacketChunk >> state.position.y;
    recievedPacketChunk >> state.velocity.x;
    recievedPacketChunk >> state.velocity.y;
    recievedPacketChunk >> state.angle;
    recievedPacketChunk >> state.angularVelocity;

    return state;
}


