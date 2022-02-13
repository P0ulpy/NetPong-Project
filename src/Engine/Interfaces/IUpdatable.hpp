//
// Created by Flo on 13/02/2022.
//

#ifndef NETPONG_PROJECT_IUPDATABLE_HPP
#define NETPONG_PROJECT_IUPDATABLE_HPP

namespace Engine
{
    class IUpdatable
    {
    public:
        virtual void update(const float& deltaTime) = 0;
    };
}

#endif //NETPONG_PROJECT_IUPDATABLE_HPP
