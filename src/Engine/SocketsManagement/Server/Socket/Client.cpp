#include "Client.hpp"

Client::Client(const std::string& id, sf::TcpSocket* socket)
	: _id(id)
	, _socket(socket)
{
	
}
