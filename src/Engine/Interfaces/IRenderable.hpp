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
    };
}

#endif //NETPONG_PROJECT_IRENDERABLE_HPP
