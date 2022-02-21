//
// Created by Flo on 14/02/2022.
//

#include "SyncableObject.hpp"

using namespace Client;

SyncableObject::SyncableObject(const SyncableObjectOptions& options)
    : _id(options.id)
    , _control(options.control)
    , _type(options.entityType)
    , _controllerId(options.controllerID)
{

}

const unsigned int& SyncableObject::getID() const               { return _id; }
const SyncableObjectControl& SyncableObject::getControl() const { return _control; }
const SyncableObjectType& SyncableObject::getType() const       { return _type; }
const std::string &SyncableObject::getControllerId() const      { return _controllerId; }

SyncableObject::~SyncableObject() {

}