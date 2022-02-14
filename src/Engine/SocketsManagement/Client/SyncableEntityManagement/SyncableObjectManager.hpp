//
// Created by Flo on 14/02/2022.
//

#ifndef NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP
#define NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP

#include <SFML/System/Thread.hpp>
#include <mutex>
#include "SFML/Network/Packet.hpp"

class ClientSocket;

namespace Client
{
    class SyncableObject;
    enum SyncableObjectControl;
    enum SyncableObjectType;

    class SyncableObjectManager
    {
    public:
        explicit SyncableObjectManager(ClientSocket *clientSocket);
        ~SyncableObjectManager();

    private:
        ClientSocket* _clientSocket = nullptr;

        std::mutex _mutex;
        sf::Thread _syncThread = sf::Thread(&SyncableObjectManager::syncThreadEntry, this);

        [[noreturn]] void syncThreadEntry();

        std::vector<SyncableObject*> _syncableObjects;

        void onSceneUpdate(sf::Packet& packet);
        SyncableObject* createEntity(unsigned int id, SyncableObjectControl control, SyncableObjectType entityType);
        void deleteEntity(unsigned int id);

        friend ClientSocket;
    };
}


#endif //NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP
