//
// Created by Flo on 11/02/2022.
//

#ifndef NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP
#define NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP

#include "../ControllerBase.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

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

    explicit LocalCharacterController(Character &character, const KeyMap& keymap);
    void update(const float& deltaTime) final;

    float calcRotFromMousePos(sf::Vector2i mousePos);

private:
    KeyMap _keyMap;

    void rotate();
    void translate(const float& deltaTime);
    void shoot();
};

#endif //NETPONG_PROJECT_LOCALCHARACTERCONTROLLER_HPP
