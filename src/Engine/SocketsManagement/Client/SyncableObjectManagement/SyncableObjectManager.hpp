//
// Created by Flo on 14/02/2022.
//

#ifndef NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP
#define NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP

#include <SFML/System/Thread.hpp>
#include "SFML/Network/Packet.hpp"
#include "../../SyncableObjectType.hpp"
#include "../../SyncableObjectOptions.hpp"
#include <mutex>
#include <map>

class ClientSocket;

namespace Client
{
    class SyncableObject;

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

        std::map<int, SyncableObject*> _syncableObjects;

        void onSceneUpdate(sf::Packet& packet);
        SyncableObject* createEntity(SyncableObjectOptions options);
        void deleteEntity(unsigned int id);

        friend ClientSocket;
    };
}


#endif //NETPONG_PROJECT_SYNCABLEOBJECTMANAGER_HPP
