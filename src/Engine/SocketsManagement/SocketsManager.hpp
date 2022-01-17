#pragma once
#include <string>

class PoPossibEngin;
class Server;
class ClientSocket;

struct HostSettings
{
	std::string name = "New Lobby";
	int port = 25565;
	float socketConnectionTimeout = 2000.f;

	HostSettings(std::string pName = "New Lobby", int pPort = 25565, float pSocketConnectionTimeout = 2000.f)
		: name(pName), port(pPort), socketConnectionTimeout(pSocketConnectionTimeout) {}
};

struct ClientConnectionSettings
{
	std::string ip = "127.0.0.1";
	int port = 25565;

	ClientConnectionSettings(std::string pIP = "127.0.0.1", int pPort = 25565)
		: ip(pIP), port(pPort) {}
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
	[[nodiscard]] const bool isHost() const;
	[[nodiscard]] const bool isClient() const;

private:
	PoPossibEngin& _engine;

	ClientSocket* _socketClient = nullptr;
	Server* _serverInstance = nullptr;
};