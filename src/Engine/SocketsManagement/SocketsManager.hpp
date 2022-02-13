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
	std::string name = "New Lobby";
    uint16_t port = 25565;
	float socketConnectionTimeout = 2000.f;

	explicit HostSettings(std::string pName = "New Lobby", uint16_t pPort = 25565, float pSocketConnectionTimeout = 2000.f)
		: name(std::move(pName))
        , port(pPort)
        , socketConnectionTimeout(pSocketConnectionTimeout) {}
};

struct ClientConnectionSettings
{
    std::string ip = "127.0.0.1";
    uint16_t port = 25565;
	int32_t connectionTimeout = 1000;

    explicit ClientConnectionSettings(std::string pIP = "127.0.0.1", uint16_t pPort = 25565, int32_t connectionTimeout = 1000)
            : ip(std::move(pIP))
            , port(pPort)
			, connectionTimeout(connectionTimeout) {}
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
	[[nodiscard]] Server::ServerMain* getServerInstance() const;
	[[nodiscard]] bool isHost() const;
	[[nodiscard]] bool isClient() const;

private:
	PoPossibEngin& _engine;

	ClientSocket* _socketClient = nullptr;
	Server::ServerMain* _serverInstance = nullptr;
};