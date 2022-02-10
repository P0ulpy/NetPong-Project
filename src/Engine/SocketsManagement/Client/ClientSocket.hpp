#pragma once

#include <SFML/Network.hpp>
#include <SFML/System/Thread.hpp>

#include "../SocketsManager.hpp"
#include "../../../Utils/EventEmitter.hpp"
#include "../SocketEvents.hpp"

struct ClientConnectionSettings;
class PoPossibEngin;

class ClientSocket
{
public:
	ClientSocket(const ClientConnectionSettings& clientConnectionSettings, PoPossibEngin* engine);
	~ClientSocket();

    // asynchronously fire event in socket server
    void send(SocketEvents event, const sf::Packet& data);

	[[nodiscard]] const ClientConnectionSettings& getClientConnectionSettings() const;
	[[nodiscard]] const EventEmitter& getEventEmitter() const;

private:
	std::string _id = "undefined";

	ClientConnectionSettings _clientConnectionSettings;
	PoPossibEngin* _engine = nullptr;

    sf::TcpSocket _socket;
	//sf::UdpSocket _socket;

	EventEmitter _eventEmitter;
	void registerListeners();

    std::mutex _mutex;
	sf::Thread _listenThread;

    [[noreturn]] void listenEvents();

	void onConnected(sf::Packet packet);
};
