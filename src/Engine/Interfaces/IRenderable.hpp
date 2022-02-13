//
// Created by Flo on 13/02/2022.
//

#ifndef NETPONG_PROJECT_IRENDERABLE_HPP
#define NETPONG_PROJECT_IRENDERABLE_HPP

#include <SFML/System/Vector2.hpp>

namespace sf {
    class RenderTarget;
}

namespace Engine
{
    class IRenderable
    {
    public:
        virtual void render(sf::RenderTarget& renderTarget) const = 0;
        virtual void setPosition(int xSpawn, int ySpawn) = 0;
        virtual void setRotation(float rot) = 0;
        virtual void setVelocity(const sf::Vector2f& newVelocity) = 0;
    };
}

#endif //NETPONG_PROJECT_IRENDERABLE_HPP
