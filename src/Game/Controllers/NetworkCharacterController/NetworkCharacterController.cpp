//
// Created by Flo on 11/02/2022.
//

#include "NetworkCharacterController.hpp"
#include "../../Entities/Character.hpp"

PlayerState::PlayerState(const sf::Vector2i& position, const sf::Vector2f &velocity, float angle, float angularVelocity)
        : velocity(velocity), position(position), angle(angle), angularVelocity(angularVelocity) {}

NetworkCharacterController::NetworkCharacterController(Character &character)
        : _character(character) {}

void NetworkCharacterController::onReceive(const PlayerState &playerState)
{
    _lastPlayerState = playerState;
    _netDelta = 0;
}

void NetworkCharacterController::Update(float dt)
{
    _netDelta += dt;
    // TODO : Maybe have a very quick Lerp instead of instant rotation
    _character.setRotation(_lastPlayerState.angle);
    _character.setVelocity(positionPrediction());
}

sf::Vector2f NetworkCharacterController::positionPrediction() const
{
    //float scale = (100 - ((_maxNetDelta * 100) / (_netDelta * 100))) / 100;

    // TEMP
    float scale = _netDelta >= _maxNetDelta ? 0 : 1;

    return {
            _lastPlayerState.velocity.x * scale,
            _lastPlayerState.velocity.y * scale
    };
}

PlayerState NetworkCharacterController::getCurrentPlayerState() const
{
    sf::Vector2f velocity;
    float angle;

    return {
        (sf::Vector2i)_character.getShape().getPosition(),
        _character.getVelocity(),
        _character.getRotation(),
        //TEMP
        0
    };
}

const PlayerState &NetworkCharacterController::getLastPlayerState() const { return _lastPlayerState; }
double NetworkCharacterController::getTimeSinceLastReceive() const { return _netDelta; }

