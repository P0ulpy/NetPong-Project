//
// Created by Flo on 11/02/2022.
//

#include "NetworkCharacterController.hpp"
#include "../../Entities/Character.hpp"

PlayerState::PlayerState(const sf::Vector2i& position, const sf::Vector2f &velocity, float angle, float angularVelocity)
        : velocity(velocity), position(position), angle(angle), angularVelocity(angularVelocity) {}

NetworkCharacterController::NetworkCharacterController(Client::SyncableObjectOptions options, Character& controlTarget)
        : Client::SyncableObject(options)
        , ControllerBase(controlTarget) {}

void NetworkCharacterController::onReceive(const PlayerState &playerState)
{
    _lastPlayerState = playerState;
    _netDelta = 0;
    rotate();
    // TODO : correcting trajectory instead of TP
    _controlTarget.setPosition(playerState.position);
}

void NetworkCharacterController::update(const float& deltaTime)
{
    _netDelta += deltaTime;

    rotate();
    translate(deltaTime);
}

void NetworkCharacterController::rotate()
{
    // TODO : Maybe have a very quick Lerp instead of instant rotation
    _controlTarget.setRotation(_lastPlayerState.angle);
}

void NetworkCharacterController::translate(const float &deltaTime)
{
    _controlTarget.setVelocity(velocityPrediction());
}

void NetworkCharacterController::shoot()
{

}

sf::Vector2f NetworkCharacterController::velocityPrediction() const
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
    return {
        (sf::Vector2i)_controlTarget.getPosition(),
        _controlTarget.getVelocity(),
        _controlTarget.getRotation(),
        //TEMP
        0
    };
}

const PlayerState &NetworkCharacterController::getLastPlayerState() const { return _lastPlayerState; }
double NetworkCharacterController::getTimeSinceLastReceive() const { return _netDelta; }

