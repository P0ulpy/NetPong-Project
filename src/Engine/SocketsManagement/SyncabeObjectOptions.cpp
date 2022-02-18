//
// Created by Flo on 17/02/2022.
//


#include <utility>

#include "SyncableObjectOptions.hpp"

SyncableObjectOptions::SyncableObjectOptions(int id, SyncableObjectType entityType, SyncableObjectControl control, std::string controllerID)
        : id(id)
        , entityType(entityType)
        , control(control)
        , controllerID(std::move(controllerID))
{}