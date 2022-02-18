//
// Created by Flo on 13/02/2022.
//

#ifndef NETPONG_PROJECT_CONTROLLERBASE_HPP
#define NETPONG_PROJECT_CONTROLLERBASE_HPP

#include "../../Engine/Interfaces/IControllable.hpp"
#include "../../Engine/Interfaces/IUpdatable.hpp"
#include "../../Engine/SocketsManagement/Client/SyncableObjectManagement/SyncableObject.hpp"

namespace Client
{
    class SyncableObject;
}

namespace Engine
{
    class ControllerBase : public Client::SyncableObject, public Engine::IUpdatable
    {
    public:
        ControllerBase(SyncableObjectOptions options, Engine::IControllable& controlTarget);
    protected:
        Engine::IControllable& _controlTarget;
    };
}


#endif //NETPONG_PROJECT_CONTROLLERBASE_HPP
