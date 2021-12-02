#pragma once
#include <string>

class PoPossibEngin;
class Server;
class ClientSocket;

struct HostSettings
{
	std::string name = "New Lobby";
	int port = 25565;
};

struct ClientConnectionSettings
{
	std::string ip = "127.0.0.1";
	int port = 25565;
};

class SocketManager
{
public:
	SocketManager(PoPossibEngin& engine);
	~SocketManager();

	void startServer(const HostSettings& hostSettings);
	void connectClient(const ClientConnectionSettings& clientConnectionSettings);

	[[nodiscard]] PoPossibEngin& getEngine() const;

	[[nodiscard]] ClientSocket* getSocketClient() const;
	[[nodiscard]] Server* getServerInstance() const;

private:
	PoPossibEngin& _engine;

	ClientSocket* _socketClient = nullptr;
	Server* _serverInstance = nullptr;
};