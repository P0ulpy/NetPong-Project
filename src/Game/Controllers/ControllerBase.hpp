//
// Created by Flo on 13/02/2022.
//

#ifndef NETPONG_PROJECT_CONTROLLERBASE_HPP
#define NETPONG_PROJECT_CONTROLLERBASE_HPP

#include "../../Engine/Interfaces/IControllable.hpp"
#include "../../Engine/Interfaces/IUpdatable.hpp"

namespace Engine
{
    class ControllerBase : public Engine::IUpdatable
    {
    protected:
        Engine::IControllable& _controlTarget;

    public:
        ControllerBase(Engine::IControllable& controlTarget);
    };
}


#endif //NETPONG_PROJECT_CONTROLLERBASE_HPP
