#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Thread.hpp>

#include "../SocketsManager.hpp"
#include "../../../Utils/EventEmitter.hpp"
#include "./SyncableObjectManagement/SyncableObjectManager.hpp"
#include "../SocketEvents.hpp"
#include "PlayerSettings.hpp"

struct ClientConnectionSettings;
class PoPossibEngin;

class ClientSocket
{
public:
	ClientSocket(ClientConnectionSettings  clientConnectionSettings, PoPossibEngin* engine);
	~ClientSocket();

    // asynchronously send packet to socket server
    void send(SocketEvents event, const sf::Packet& data);

	[[nodiscard]] const ClientConnectionSettings& getClientConnectionSettings() const;
    [[nodiscard]] const EventEmitter* getEventEmitter() const;
    [[nodiscard]] Client::SyncableObjectManager& getSyncableObjectManager();
    [[nodiscard]] bool isReady() const;

    void waitReady() const;

    // TODO: Proprifier (get / set ect)
    PlayerSettings _playerSettings;

private:
	std::string _id = "undefined";
    bool _ready = false;

    ClientConnectionSettings _clientConnectionSettings;
	PoPossibEngin* _engine = nullptr;

    sf::TcpSocket _socket;
	//sf::UdpSocket _socket;

    Client::SyncableObjectManager _syncableObjectManager;

    EventEmitter _eventEmitter;
	void registerListeners();

    std::mutex _mutex;
	sf::Thread _listenThread;

    [[noreturn]] void listenEvents();

    void sendPlayerSettings();

	void onConnected(sf::Packet packet);
};
