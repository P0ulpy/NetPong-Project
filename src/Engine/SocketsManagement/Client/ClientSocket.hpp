#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Thread.hpp>

#include "../SocketsManager.hpp"
#include "../../../Utils/EventEmitter.hpp"

struct ClientConnectionSettings;
class PoPossibEngin;

class ClientSocket
{
public:
	ClientSocket(const ClientConnectionSettings& clientConnectionSettings, PoPossibEngin* engine);
	~ClientSocket();

	[[nodiscard]] const ClientConnectionSettings& getClientConnectionSettings() const;
	[[nodiscard]] const EventEmitter& getEventEmitter() const;

private:
	ClientConnectionSettings _clientConnectionSettings;
	PoPossibEngin* _engine = nullptr;

	sf::TcpSocket _socket;

	EventEmitter _eventEmitter;
	void registerListeners();

	sf::Thread _listenThread;
	void listenEvents();

	void onConnected(sf::Packet packet);
};
