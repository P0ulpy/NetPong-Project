#pragma once

#include <iostream>
#include "../../../../Utils/EventEmitter.hpp"
#include "../../SocketEvents.hpp"

namespace sf {
	class TcpSocket;
}

class Client : protected EventEmitter
{
public:
	Client(const std::string& id, sf::TcpSocket* socket);

	template <typename... Args>
	unsigned int on(SocketEvents event_id, std::function<void(Args...)> cb) { add_listener(event_id, cb); }

	unsigned int on(SocketEvents event_id, std::function<void()> cb)		{ add_listener(event_id, cb); }

	template<typename LambdaType>
	unsigned int on(SocketEvents event_id, LambdaType lambda)				{ add_listener(event_id, cb); }

	[[nodiscard]] const sf::TcpSocket* getSocket() const					{ return _socket; }
	[[nodiscard]] const std::string& getId() const							{ return _id; }

private:
	std::string _id = "";
	sf::TcpSocket* _socket = nullptr;
};