//
// Created by Flo on 14/02/2022.
//

#ifndef NETPONG_PROJECT_SYNCABLEOBJECT_HPP
#define NETPONG_PROJECT_SYNCABLEOBJECT_HPP

#include "../ClientSocket.hpp"
#include "./SyncableObjectManager.hpp"

namespace Client
{
    enum SyncableObjectControl { Local, Remote };

    enum SyncableObjectType {
        Character
    };

    struct SyncableObjectOptions
    {
        SyncableObjectOptions(unsigned int id, SyncableObjectControl control, SyncableObjectType entityType);
        unsigned int id;
        SyncableObjectControl control;
        SyncableObjectType entityType;
    };

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

        virtual void sync(sf::Packet& sendPacketChunck) = 0;
        virtual void applySync(sf::Packet& recievedPacketChunk) = 0;

        friend SyncableObjectManager;
    };
}

#endif //NETPONG_PROJECT_SYNCABLEOBJECT_HPP
