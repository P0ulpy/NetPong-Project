//
// Created by Flo on 15/02/2022.
//

#ifndef NETPONG_PROJECT_SYNCABLEOBJECT_HPP
#define NETPONG_PROJECT_SYNCABLEOBJECT_HPP

#include "../../SyncableObjectType.hpp"
#include "../Objects/ISyncable.hpp"
#include "../Socket/Client.hpp"
#include "../../SyncableObjectOptions.hpp"
#include <string>


namespace Server
{
    class SyncableObject
    {
    public:
        SyncableObject(unsigned int id, SyncableObjectType type, Server::ISyncable *object, Server::Client &controller);

        unsigned int& getId();
        SyncableObjectType& getType();
        ISyncable* getObject();
        Client& getController();
    private:
        unsigned int _id;
        SyncableObjectType _type;
        SyncableObjectControl _control;
        ISyncable* _object = nullptr;
        Client& _controller;
    };
}

#endif //NETPONG_PROJECT_SYNCABLEOBJECT_HPP
