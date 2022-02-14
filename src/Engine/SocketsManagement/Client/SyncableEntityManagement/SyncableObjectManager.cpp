//
// Created by Flo on 14/02/2022.
//

#include "SyncableObjectManager.hpp"
#include "../ClientSocket.hpp"
#include "./SyncableObject.hpp"
#include "../../../../Logger/Logger.hpp"
#include "../../../Scenes/MainGameScene.hpp"

using namespace Client;

constexpr unsigned int tickDelay = 10;

SyncableObjectManager::SyncableObjectManager(ClientSocket *clientSocket)
    : _clientSocket(clientSocket)
{
    _syncThread.launch();
}

SyncableObjectManager::~SyncableObjectManager() {
    _syncThread.terminate();
}

void SyncableObjectManager::syncThreadEntry()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(tickDelay));
    }
}

void SyncableObjectManager::onSceneUpdate(sf::Packet &packet)
{
    int entityID;
    if(packet >> entityID)
    {
        /*if(_syncableObjects[entityID] == nullptr)
        {
            _syncableObjects[entityID]
        }*/
    }
    else
    {
        Logger::Log("c'est la fin");
    }
}

SyncableObject* SyncableObjectManager::createEntity(unsigned int id, SyncableObjectControl control, SyncableObjectType entityType)
{
    auto scene = MainGameScene::getInstance();
    if(!scene)
    {
        Logger::Err("MainGameScene is not loaded, can't create syncable entity");
        return nullptr;
    }

    switch (entityType)
    {
        case Character:
            //scene.createPlayer(id, control, entityType);
            break;

        default:
            Logger::Warn(&"UnHandled entity creation | TYPE: " [entityType]);
    }
}

void SyncableObjectManager::deleteEntity(unsigned int id)
{

}



