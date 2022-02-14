//
// Created by Flo on 14/02/2022.
//

#include "SyncableObject.hpp"

using namespace Client;

SyncableObjectOptions::SyncableObjectOptions(unsigned int id, SyncableObjectControl control, SyncableObjectType entityType)
        : id(id)
        , control(control)
        , entityType(entityType)
        {}

SyncableObject::SyncableObject(const SyncableObjectOptions& options)
    : _id(options.id)
    , _control(options.control)
    , _type(options.entityType)
{

}

const unsigned int& SyncableObject::getID() const { return _id; }
const SyncableObjectControl& SyncableObject::getControl() const { return _control; }
const SyncableObjectType& SyncableObject::getType() const { return _type; }
