#pragma once
#include <string>
#include <utility>

class PoPossibEngin;
class ClientSocket;

namespace Server {
    class ServerMain;
}

struct HostSettings
{
	std::string name;
    uint16_t port;
	float socketConnectionTimeout;

	HostSettings(std::string pName = "New Lobby", uint16_t pPort = 25565, float pSocketConnectionTimeout = 2000.f)
		: name(std::move(pName))
        , port(pPort)
        , socketConnectionTimeout(pSocketConnectionTimeout) {}
};

struct ClientConnectionSettings
{
    std::string ip;
    uint16_t port;
    std::string pseudo;
	int32_t connectionTimeout;

    ClientConnectionSettings(std::string pIP = "127.0.0.1", uint16_t pPort = 25565, std::string pseudo = "UnnamedPlayer", int32_t connectionTimeout = 1000)
            : ip(std::move(pIP))
            , port(pPort)
			, connectionTimeout(connectionTimeout)
            , pseudo(std::move(pseudo)) {}
};

class SocketManager
{
public:
	SocketManager(PoPossibEngin& engine);
	~SocketManager();

	void startServer(const HostSettings& hostSettings);
	ClientSocket* connectClient(const ClientConnectionSettings& clientConnectionSettings);

	[[nodiscard]] PoPossibEngin& getEngine() const;

	[[nodiscard]] ClientSocket* getSocketClient() const;
	[[nodiscard]] Server::ServerMain* getServerInstance() const;
	[[nodiscard]] bool isHost() const;
	[[nodiscard]] bool isClient() const;

    void disconnectAll();

private:
	PoPossibEngin& _engine;

	ClientSocket* _socketClient = nullptr;
	Server::ServerMain* _serverInstance = nullptr;
};