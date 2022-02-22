//
// Created by Flo on 11/02/2022.
//

#include "NetworkCharacterController.hpp"
#include "../../Entities/Character.hpp"
#include "../../../Engine/Scenes/MainGameScene.hpp"
#include "../../Entities/PongBall.hpp"

NetworkCharacterController::NetworkCharacterController(SyncableObjectOptions options, Character& controlTarget)
        : ControllerBase(options, controlTarget)
        , _character(controlTarget)
        {}

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

sf::Vector2f NetworkCharacterController::velocityPrediction()
{
    //float scale = (100 - ((_maxNetDelta * 100) / (_netDelta * 100))) / 100;

    /*
     * Po = where it is now, the old position
     * P'o = last know state position
     * Vo = the old velocity
     * V'o = the last known velocity
     * A'o = acceleration
     * TΔ = How much update per second for network ?
     * Tt = Time total since the last update (netDelta)
     * Tf = Time since the last frame (deltaTime)
     * T^ = Normalized time -> T^ = Tt / TΔ
     *
     * Vb = new blended velocity -> Vb = Vo + (V'o - Vo) * T^
     * Pt = projecting from where we where -> Pt = Po + Vb * Tt + 0.5f * A'o * std::pow(Tt, 2)
     * P't = projecting from last known -> P't = P'o + V'o * Tt + 0.5f * A'o *  std::pow(Tt, 2)
     * Qt = Pt + (P't - Pt) * T^
     */

    //const float normalizedDeltaTime = _netDelta / SERVER_TICK_RATE;
    //const sf::Vector2f lastPlayerStatePositionToInt = sf::Vector2f(static_cast<float>(_lastPlayerState.position.x),static_cast<float>(_lastPlayerState.position.y));

    //const sf::Vector2f velocityBlended = _oldVelocity + (_lastPlayerState.velocity - _oldVelocity) * normalizedDeltaTime;
    //const sf::Vector2f projectionOld = _oldPosition + velocityBlended * _netDelta;// + 0.5f * A'o * std::pow(Tt, 2)
    //const sf::Vector2f projectionLastKnown = lastPlayerStatePositionToInt + _lastPlayerState.velocity * _netDelta;// + 0.5f * A'o * std::pow(Tt, 2)
    //const sf::Vector2f combinedProjections = projectionOld + (projectionLastKnown - projectionOld) * normalizedDeltaTime;

    //_oldPosition = lastPlayerStatePositionToInt;
    //_oldVelocity = _lastPlayerState.velocity;

    //return {
    //        combinedProjections.x,
    //        combinedProjections.y
    //};

    return {
        _lastPlayerState.velocity.x,
        _lastPlayerState.velocity.y
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
    //_controlTarget.setPosition(_lastPlayerState.position);

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

