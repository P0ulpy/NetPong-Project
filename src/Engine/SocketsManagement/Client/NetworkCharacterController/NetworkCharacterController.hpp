//
// Created by Flo on 11/02/2022.
//

#ifndef NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP
#define NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP

#include "SFML/System/Vector2.hpp"
#include "../ClientSocket.hpp"
#include "../../../../Game/Entities/Character.hpp"

struct PlayerState
{
    PlayerState(sf::Vector2i position, const sf::Vector2f &velocity);

    sf::Vector2i position;
    sf::Vector2f velocity;

    float angle;
    float angularVelocity;
};

class NetworkCharacterController
{
private:
    const float _maxNetDelta = 2.0;
    PlayerState _lastPlayerState;
    float _netDelta = 0;
    Character& _character;

public:
    NetworkCharacterController(Character &character);

    [[nodiscard]] const PlayerState &getLastPlayerState() const;
    [[nodiscard]] double getTimeSinceLastReceive() const;
    void Update(float dt);
private:
    void onReceive(const PlayerState& playerState);
    sf::Vector2f positionPrediction() const;
    friend ClientSocket;

};


#endif //NETPONG_PROJECT_NETWORKCHARACTERCONTROLLER_HPP
