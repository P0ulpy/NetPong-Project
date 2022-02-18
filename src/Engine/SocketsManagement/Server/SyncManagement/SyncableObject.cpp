//
// Created by Flo on 15/02/2022.
//

#include "SyncableObject.hpp"

Server::SyncableObject::SyncableObject(unsigned int id, SyncableObjectType type, Server::ISyncable *object, Server::Client &controller)
        : _id(id)
        , _type(type)
        , _object(object)
        , _controller(controller) {}

unsigned int &Server::SyncableObject::getId() { return _id; }
SyncableObjectType &Server::SyncableObject::getType() { return _type; }
Server::ISyncable* Server::SyncableObject::getObject() { return _object; }
Server::Client &Server::SyncableObject::getController() { return _controller; }
