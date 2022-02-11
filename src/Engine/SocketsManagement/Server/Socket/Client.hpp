#pragma once

#include <iostream>
#include "../../../../Utils/EventEmitter.hpp"
#include "../../SocketEvents.hpp"
#include "../../Client/PlayerSettings.hpp"

namespace sf {
	class TcpSocket;
}

namespace Server
{
    class Client : public EventEmitter {
    public:
        Client(const std::string &id, sf::TcpSocket *socket);
        Client(const Client &) = delete;

        const Client &operator=(const Client &) = delete;

        ~Client();

        [[nodiscard]] sf::TcpSocket *getSocket() const;
        [[nodiscard]] const std::string &getId() const;
        [[nodiscard]] const PlayerSettings &getSettings() const;
        void setSettings(const PlayerSettings &settings);

    private:
        std::string _id;
        sf::TcpSocket *_socket = nullptr;

        PlayerSettings settings;
    };
}