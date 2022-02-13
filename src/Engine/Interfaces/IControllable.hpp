//
// Created by Flo on 13/02/2022.
//

#ifndef NETPONG_PROJECT_ICONTROLLABLE_HPP
#define NETPONG_PROJECT_ICONTROLLABLE_HPP

#include "IUpdatable.hpp"
#include <SFML/System/Vector2.hpp>

namespace Engine
{
    class IControllable : public IUpdatable
    {
    public:
        [[nodiscard]] virtual const sf::Vector2f& getVelocity() = 0;
        [[nodiscard]] virtual const sf::Vector2f& getPosition() const = 0;
        [[nodiscard]] virtual float getRotation() const = 0;

        virtual void setPosition(int xSpawn, int ySpawn) = 0;
        virtual void setRotation(float rot) = 0;
        virtual void setVelocity(const sf::Vector2f& newVelocity) = 0;
    };
}

#endif //NETPONG_PROJECT_ICONTROLLABLE_HPP
