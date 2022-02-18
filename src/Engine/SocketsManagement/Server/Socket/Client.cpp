#include "Client.hpp"
#include "../../../../Logger/Logger.hpp"
#include <SFML/Network.hpp>
#include <utility>

using namespace Server;

Client::Client(std::string id, sf::TcpSocket* socket)
	: _id(std::move(id))
	, _socket(socket)
{
	
}

Client::~Client()
{
    Logger::Log("Deleting Socket :" + _id);
    delete _socket;
}

sf::TcpSocket *Client::getSocket() const { return _socket; }
std::string &Client::getId() { return _id; }
const PlayerSettings &Client::getSettings() const { return settings; }
void Client::setSettings(const PlayerSettings &settings) { Client::settings = settings; }
