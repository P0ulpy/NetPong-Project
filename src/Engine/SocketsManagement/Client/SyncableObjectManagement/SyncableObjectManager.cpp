//
// Created by Flo on 14/02/2022.
//

#include "SyncableObjectManager.hpp"
#include "../ClientSocket.hpp"
#include "./SyncableObject.hpp"
#include "../../../../Logger/Logger.hpp"
#include "../../../Scenes/MainGameScene.hpp"

using namespace Client;

constexpr unsigned int tickDelay = 1000;

SyncableObjectManager::SyncableObjectManager(ClientSocket *clientSocket)
    : _clientSocket(clientSocket)
{
    _syncThread.launch();

}

SyncableObjectManager::~SyncableObjectManager()
{
    _syncThread.terminate();
}

void SyncableObjectManager::syncThreadEntry()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(tickDelay));

        {
            sf::Packet sceneState;

            for(auto objectPair : _syncableObjects)
            {
                auto object = objectPair.second;

                sceneState << object->getID();
                sceneState << object->getType();

                Logger::Log("");

                std::stringstream debugStream;
                debugStream << "Sending :\n{ id:" << object->getID() << ", " << "type:" << object->getType() << ", data: {\n     ";
                sceneState << object->sync(debugStream);
                debugStream << " }\n}";
                Logger::Log(debugStream.str());
            }
        }
    }
}

void SyncableObjectManager::onSceneUpdate(sf::Packet &packet)
{
    while(!packet.endOfPacket())
    {
        unsigned int id;
        int typeInt;
        SyncableObjectType type;

        packet >> id;
        packet >> typeInt;
        type = (SyncableObjectType)typeInt;

        if(id >= 0)
        {
            if(_syncableObjects.contains(id))
            {
                std::lock_guard guard(_mutex);

                std::stringstream debugStream;
                debugStream << "Receiving :\n{\n     id:" << id << ", " << "type:" << type << ", data: {\n         ";
                _syncableObjects[id]->applySync(packet, debugStream);
                debugStream << "    \n}\n}";
                Logger::Log(debugStream.str());
            }
            else
            {
                createEntity(id, SyncableObjectControl::Local, type);
            }
        }
        else
        {
            Logger::Err("Invalid syncable Object id : " + std::to_string(id));
        }
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

    SyncableObject* newObject = nullptr;

    switch (entityType)
    {
        case CharacterType:
            newObject = scene->createPlayer({ id, control, entityType });
            break;

        default:
            Logger::Warn("UnHandled entity creation | TYPE: " + std::to_string(entityType));
    }

    _syncableObjects[id] = newObject;
    return newObject;
}

void SyncableObjectManager::deleteEntity(unsigned int id)
{

}



