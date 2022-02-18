//
// Created by Flo on 14/02/2022.
//

#include "SyncableObjectManager.hpp"
#include "../ClientSocket.hpp"
#include "../../../../Logger/Logger.hpp"
#include "../../../Scenes/MainGameScene.hpp"

using namespace Client;

constexpr unsigned int tickDelay = 30;

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
    Logger::SetThreadLabel("Client-SyncThread");

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(tickDelay));

        {
            sf::Packet sceneState;
            std::lock_guard guard(_mutex);

            for(auto objectPair : _syncableObjects)
            {
                auto object = objectPair.second;

                if(!object)
                {
                    Logger::Err("try to sync null object id :" + std::to_string(objectPair.first));
                    continue;
                }

                if(object->getControl() != SyncableObjectControl::Local)
                    continue;

                sceneState << object->getID();
                sceneState << object->getType();
                sceneState << object->getControl();

                std::stringstream debugStream;
                debugStream << "Sending :\n{id:" << object->getID() << ", " << "type:" << object->getType() << ", control:" << object->getControl() << ", data: {";

                sf::Packet packet { object->sync(debugStream) };
                sceneState.append(packet.getData(), packet.getDataSize());

                debugStream << "} }";
                //Logger::Log(debugStream.str());

                _clientSocket->send(SocketEvents::SceneUpdate, sceneState);
            }
        }
    }
}

void SyncableObjectManager::onSceneUpdate(sf::Packet &packet)
{
    while(!packet.endOfPacket())
    {
        int id = -1;
        packet >> id;

        int typeInt;
        packet >> typeInt;
        auto type = (SyncableObjectType)typeInt;

        std::string controllerID;
        packet >> controllerID;
        SyncableObjectControl control = (controllerID == _clientSocket->getId()) ? SyncableObjectControl::Local : SyncableObjectControl::Remote;

        if(id < 0)
        {
            //Logger::Err("Invalid syncable Object id : " + std::to_string(id));
            return;
        }

        std::lock_guard guard(_mutex);

        std::stringstream debugStream;
        debugStream << "Receiving :\n{id:" << id << ", " << "type:" << type << ", control:" << control <<", data: {";

        if(!_syncableObjects.contains(id) || !_syncableObjects[id])
        {
            createEntity({id, type, control, controllerID});
        }

        if(_syncableObjects[id])
        {
            _syncableObjects[id]->applySync(packet, debugStream);
        }
        else
        {
            //Logger::Err("Can't applySync : value in pool is nullptr id:" + std::to_string(id));
        }

        debugStream << "} }";
        //Logger::Log(debugStream.str());
    }
}

SyncableObject* SyncableObjectManager::createEntity(SyncableObjectOptions options)
{
    auto scene = MainGameScene::getInstance();



    if(!scene)
    {
        Logger::Err("MainGameScene is not loaded, can't create syncable entity");
        return nullptr;
    }

    SyncableObject* newObject = nullptr;

    switch (options.entityType)
    {
        case CharacterType:
            newObject = scene->createPlayer(options);
            break;

        default:
            Logger::Warn("UnHandled entity creation | TYPE: " + std::to_string(options.entityType));
    }

    _syncableObjects[options.id] = newObject;
    return newObject;
}

void SyncableObjectManager::deleteEntity(unsigned int id)
{

}



