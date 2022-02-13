//
// Created by Flo on 11/02/2022.
//

#ifndef NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP
#define NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP

#include "SFML/System/Vector2.hpp"
#include "../../../Engine/SocketsManagement/Client/ClientSocket.hpp"

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

class NetworkCharacterController
{
private:
    Character& _character;

    const float _maxNetDelta = 2.0;
    PlayerState _lastPlayerState;
    float _netDelta = 0;

public:
    NetworkCharacterController(Character &character);

    [[nodiscard]] PlayerState getCurrentPlayerState() const;
    [[nodiscard]] const PlayerState &getLastPlayerState() const;
    [[nodiscard]] double getTimeSinceLastReceive() const;
    void Update(float dt);

private:
    void onReceive(const PlayerState& playerState);
    [[nodiscard]] sf::Vector2f positionPrediction() const;

    friend ClientSocket;
};

#endif //NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP
