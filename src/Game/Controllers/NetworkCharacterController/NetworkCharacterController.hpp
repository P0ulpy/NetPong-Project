//
// Created by Flo on 11/02/2022.
//

#ifndef NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP
#define NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP

#include "SFML/System/Vector2.hpp"
#include "../../../Engine/SocketsManagement/Client/ClientSocket.hpp"
#include "../ControllerBase.hpp"
#include "../../../Engine/SocketsManagement/Client/SyncableEntityManagement/SyncableObject.hpp"

struct PlayerState
{
    PlayerState() = default;
    PlayerState(const sf::Vector2i& position, const sf::Vector2f &velocity, float angle, float angularVelocity);

    sf::Vector2i position;
    sf::Vector2f velocity;

    float angle = 0;
    float angularVelocity = 0;
};

class Character;

class NetworkCharacterController : public Engine::ControllerBase, public Client::SyncableObject
{
private:
    const float _maxNetDelta = 2.0;
    PlayerState _lastPlayerState;
    float _netDelta = 0;

public:
    explicit NetworkCharacterController(Client::SyncableObjectOptions options, Character& controlTarget);

    [[nodiscard]] PlayerState getCurrentPlayerState() const;
    [[nodiscard]] const PlayerState &getLastPlayerState() const;
    [[nodiscard]] double getTimeSinceLastReceive() const;
    void update(const float& deltaTime) final;

private:
    void onReceive(const PlayerState& playerState);

    void rotate();
    void translate(const float& deltaTime);
    void shoot();
    [[nodiscard]] sf::Vector2f velocityPrediction() const;

    friend ClientSocket;
};

#endif //NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP
