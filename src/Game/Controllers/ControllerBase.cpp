//
// Created by Flo on 13/02/2022.
//

#include "ControllerBase.hpp"

Engine::ControllerBase::ControllerBase(SyncableObjectOptions options, Engine::IControllable &controlTarget)
    : Client::SyncableObject(options)
    , Engine::IUpdatable()
    , _controlTarget(controlTarget) {}

Engine::ControllerBase::~ControllerBase() {

}

Engine::IControllable &Engine::ControllerBase::getControlTarget() const {
    return _controlTarget;
}
