#include "Client.hpp"
#include "../../../../Logger/Logger.hpp"
#include <SFML/Network.hpp>

Client::Client(const std::string& id, sf::TcpSocket* socket)
	: _id(id)
	, _socket(socket)
{
	
}

Client::~Client()
{
    Logger::Log("Deleting Socket :" + _id);
    delete _socket;
}

sf::TcpSocket *Client::getSocket() const { return _socket; }
const std::string &Client::getId() const { return _id; }