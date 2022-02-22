//
// Created by Flo on 11/02/2022.
//

#ifndef NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP
#define NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP

#include "../../../Engine/SocketsManagement/Client/ClientSocket.hpp"
#include "../ControllerBase.hpp"
#include "../../../Engine/SocketsManagement/Client/SyncableObjectManagement/SyncableObject.hpp"
#include "../../../Engine/SocketsManagement/ObjectsStates/PlayerState.hpp"

class Character;
class PongBall;

class NetworkCharacterController : public Engine::ControllerBase
{
private:
    const float _maxNetDelta = 2.0;
    PlayerState _lastPlayerState;

    sf::Vector2f _oldPosition {0,0};
    sf::Vector2f _oldVelocity {0,0};

    float _netDelta = 0;

public:
    explicit NetworkCharacterController(SyncableObjectOptions options, Character& controlTarget);

    [[nodiscard]] PlayerState getCurrentPlayerState() const;
    [[nodiscard]] const PlayerState &getLastPlayerState() const;
    [[nodiscard]] double getTimeSinceLastReceive() const;
    void update(const float& deltaTime) final;

    sf::Packet sync(std::stringstream& debugStream) final;
    void applySync(sf::Packet& recievedPacketChunk, std::stringstream& debugStream) final;

private:
    Character& _character;

    void rotate();
    void translate(const float& deltaTime);
    [[nodiscard]] sf::Vector2f velocityPrediction();
};

#endif //NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP
