//
// Created by Flo on 14/02/2022.
//

#ifndef NETPONG_PROJECT_SYNCABLEOBJECT_HPP
#define NETPONG_PROJECT_SYNCABLEOBJECT_HPP

#include "../ClientSocket.hpp"
#include "./SyncableObjectManager.hpp"
#include "../../SyncableObjectOptions.hpp"

namespace Client
{
    class SyncableObject
    {
    public:

        explicit SyncableObject(const SyncableObjectOptions& options);

        [[nodiscard]] const unsigned int& getID() const;
        [[nodiscard]] const SyncableObjectControl& getControl() const;
        [[nodiscard]] const SyncableObjectType& getType() const;

    protected:
        unsigned int _id;
        SyncableObjectControl _control;
        SyncableObjectType _type;

        std::mutex _mutex;

        virtual sf::Packet sync(std::stringstream& debugStream) = 0;
        virtual void applySync(sf::Packet& recievedPacketChunk, std::stringstream& debugStream) = 0;

        friend SyncableObjectManager;
    };
}

#endif //NETPONG_PROJECT_SYNCABLEOBJECT_HPP
