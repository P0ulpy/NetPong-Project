//
// Created by Flo on 11/02/2022.
//

#ifndef NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP
#define NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP

#include "../ControllerBase.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "../../../Engine/SocketsManagement/Client/SyncableObjectManagement/SyncableObject.hpp"
#include "../../../Engine/SocketsManagement/PlayerState.hpp"

class Character;

class LocalCharacterController : public Engine::ControllerBase
{
public:
    struct KeyMap
    {
        KeyMap() = default;
        KeyMap(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Mouse::Button shoot);

        sf::Keyboard::Key up;
        sf::Keyboard::Key down;
        sf::Keyboard::Key left;
        sf::Keyboard::Key right;
        sf::Mouse::Button shoot;
    };

    explicit LocalCharacterController(SyncableObjectOptions options, Character &character, const KeyMap &keymap);
    void update(const float& deltaTime) final;

    float calcRotFromMousePos(sf::Vector2i mousePos);

    [[nodiscard]] PlayerState getCurrentPlayerState() const;

    sf::Packet sync(std::stringstream& debugStream) final;
    void applySync(sf::Packet& recievedPacketChunk, std::stringstream& debugStream) final;

private:
    KeyMap _keyMap;

    void rotate();
    void translate(const float& deltaTime);
    void shoot();
};

#endif //NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP
