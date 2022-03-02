//
// Created by Flo on 14/02/2022.
//

#include "SyncableObjectManager.hpp"
#include "../ClientSocket.hpp"
#include "../../../../Logger/Logger.hpp"
#include "../../../Scenes/MainGameScene.hpp"
#include "../../../../Game/Entities/Character.hpp"
#include "../../../../Game/Controllers/PongBallController/NetworkPongBallController.hpp"
#include "../../../../Game/Controllers/LocalCharacterController/LocalCharacterController.hpp"

using namespace Client;
constexpr unsigned int tickDelay = 30;

std::map<SyncableObjectType, unsigned int> SyncableObjectManager::objectTypesInSceneUpdate = std::map<SyncableObjectType, unsigned int>();
bool SyncableObjectManager::isGameStarted = false;

SyncableObjectManager::SyncableObjectManager(ClientSocket *clientSocket)
    : _clientSocket(clientSocket)
{
    _syncThread.launch();
}

SyncableObjectManager::~SyncableObjectManager()
{
    _syncThread.terminate();
}

std::map<int, SyncableObject *> &SyncableObjectManager::getSyncableObjects() { return _syncableObjects; }
Character* SyncableObjectManager::getCharacter(const std::string& controllerID)
{

    for(auto& objectPair : _syncableObjects)
    {
        auto* obj = objectPair.second;
        if(obj && obj->getType() == CharacterType && obj->getControllerId() == controllerID)
        {
            return dynamic_cast<Character*>(&dynamic_cast<Engine::ControllerBase*>(obj)->getControlTarget());
        }
    }

    return nullptr;
}

Engine::ControllerBase* SyncableObjectManager::getCharacterController(const std::string& controllerID)
{
    for(auto& syncableObjects : _syncableObjects)
    {
        auto* obj = syncableObjects.second;
        if(obj->getType() == CharacterType && obj->getControllerId() == controllerID)
            return dynamic_cast<Engine::ControllerBase*>(obj);
    }

    return nullptr;
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
                    //Logger::Err("try to sync null object id :" + std::to_string(objectPair.first));
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
    SyncableObjectManager::objectTypesInSceneUpdate.clear();

    packet >> SyncableObjectManager::isGameStarted;

    if(!MainGameScene::getInstance()) return;

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

        if(!SyncableObjectManager::objectTypesInSceneUpdate.contains(type))
            SyncableObjectManager::objectTypesInSceneUpdate[type] = 1u;
        else
            SyncableObjectManager::objectTypesInSceneUpdate[type] = SyncableObjectManager::objectTypesInSceneUpdate[type] + 1u;

        std::stringstream debugStream;
        debugStream << "Receiving :\n{id:" << id << ", " << "type:" << type << ", control:" << control <<", data: {";

        if(!_syncableObjects.contains(id) || !_syncableObjects[id])
        {
            createEntity({id, type, control, controllerID}, packet);
        }
        else
        {
            if(_syncableObjects[id] && control != SyncableObjectControl::Local)
            {
                _syncableObjects[id]->applySync(packet, debugStream);
            }
            else
            {
                switch (type)
                {
                    case CharacterType:
                        LocalCharacterController::extractData(packet);
                        break;
                    case PongBallType:
                        NetworkPongBallController::extractData(packet);
                        break;
                }

                //Logger::Err("Can't applySync : value in pool is nullptr id:" + std::to_string(id));
            }
        }

        debugStream << "} }";
        //Logger::Log(debugStream.str());
    }
}

SyncableObject* SyncableObjectManager::createEntity(const SyncableObjectOptions& options, sf::Packet& packet)
{
    auto scene = MainGameScene::getInstance();

    if(!scene)
    {
        //Logger::Warn("MainGameScene is not loaded, can't create syncable entity");
        return nullptr;
    }

    SyncableObject* newObject = nullptr;

    switch (options.entityType)
    {
        case CharacterType:
        {
            PlayerState state = LocalCharacterController::extractData(packet);
            newObject = scene->createPlayer(options, state);
            break;
        }

        case PongBallType :
        {
            PongBallState state = NetworkPongBallController::extractData(packet);
            newObject = scene->createPongBall(options, state);
            break;
        }

        default:
            Logger::Warn("UnHandled entity creation | TYPE: " + std::to_string(options.entityType));
            break;
    }

    if(!newObject) return nullptr;

    _syncableObjects[options.id] = newObject;
    return newObject;
}

void SyncableObjectManager::deleteEntity(unsigned int id)
{

}



