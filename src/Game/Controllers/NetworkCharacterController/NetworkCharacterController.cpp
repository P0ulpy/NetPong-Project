//
// Created by Flo on 11/02/2022.
//

#include "NetworkCharacterController.hpp"
#include "../../Entities/Character.hpp"

NetworkCharacterController::NetworkCharacterController(SyncableObjectOptions options, Character& controlTarget)
        : ControllerBase(options, controlTarget) {}

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

void NetworkCharacterController::applySync(sf::Packet &recievedPacketChunk, std::stringstream &debugStream)
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

    _lastPlayerState = {
            {px, py},
            {vx, vy},
            angle,
            angleVel
    };

    _netDelta = 0;
    rotate();
    // TODO : correcting trajectory instead of TP
    _controlTarget.setPosition(_lastPlayerState.position);

    debugStream << "pos: {x:" << _lastPlayerState.position.x << ", y:" << _lastPlayerState.position.y << "}, vel: {x:" << _lastPlayerState.velocity.x << ", y:" << _lastPlayerState.velocity.y << "}, angle: " << _lastPlayerState.angle << ", angleVel: " << _lastPlayerState.angularVelocity;
}

sf::Packet NetworkCharacterController::sync(std::stringstream &debugStream)
{
    std::lock_guard guard(_mutex);

    sf::Packet packet;
    PlayerState state;
    packet.append(&state, sizeof(PlayerState));
    return packet;
}

