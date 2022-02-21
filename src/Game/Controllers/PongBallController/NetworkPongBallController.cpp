 //
// Created by Flo on 18/02/2022.
//

#include "NetworkPongBallController.hpp"
#include <utility>

NetworkPongBallController::NetworkPongBallController(SyncableObjectOptions options, PongBall &pongBall, const PongBallState& pongBallState)
    : ControllerBase(std::move(options), pongBall)
    , _pongBall(pongBall)
{
    std::stringstream debugStream;
    applySync(pongBallState, debugStream);
}

sf::Packet NetworkPongBallController::sync(std::stringstream &debugStream)
{
    sf::Packet pongballStatePacket;
    PongBallState state = getPongBallState();

    pongballStatePacket << state.position.x;
    pongballStatePacket << state.position.y;
    pongballStatePacket << state.velocity.x;
    pongballStatePacket << state.velocity.y;
    pongballStatePacket << state.enabled;
    pongballStatePacket << state.canKill;

    debugStream
            << "pos: {x:" << state.position.x << ", y:" << state.position.y
            << "}, vel: {x:" << state.velocity.x << ", y:" << state.velocity.y
            << "}, enabled: " << state.enabled << ", canKill: " << state.canKill;

    return pongballStatePacket;
}

void NetworkPongBallController::applySync(sf::Packet &recievedPacketChunk, std::stringstream &debugStream)
{
    PongBallState state = NetworkPongBallController::extractData(recievedPacketChunk);
    applySync(state, debugStream);
}

void NetworkPongBallController::applySync(const PongBallState& state, std::stringstream& debugStream)
{
    _controlTarget.setPosition(state.position);
    _controlTarget.setVelocity(state.velocity);

    if(!_pongBall.isActive())
        _pongBall.setActive(state.enabled);

    if(!_pongBall.canKill())
        _pongBall.setCanKill(state.canKill);

    debugStream
        << "pos: {x:" << state.position.x << ", y:" << state.position.y
        << "}, vel: {x:" << state.velocity.x << ", y:" << state.velocity.y
        << "}, enabled: " << state.enabled << ", canKill: " << state.canKill;
}

void NetworkPongBallController::update(const float &deltaTime)
{

}

PongBallState NetworkPongBallController::getPongBallState()
{
    return PongBallState {
        _pongBall.getPosition(),
        _pongBall.getVelocity(),
        _pongBall.isActive(),
        _pongBall.canKill()
    };
}


PongBallState NetworkPongBallController::extractData(sf::Packet &recievedPacketChunk)
{
    PongBallState state;

    recievedPacketChunk >> state.position.x;
    recievedPacketChunk >> state.position.y;
    recievedPacketChunk >> state.velocity.x;
    recievedPacketChunk >> state.velocity.y;
    recievedPacketChunk >> state.enabled;
    recievedPacketChunk >> state.canKill;

    return state;
}
