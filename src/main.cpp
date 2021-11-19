#include "Engine/Engine.hpp"

/*
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <imgui.h>
#include <imgui-SFML.h>
*/

void client();
void server();

int main(int argc, char** argv)
{
    PoPossibEngin engine = PoPossibEngin(
        EngineConfig(
            EngineConfig::WindowConfig(
	            sf::VideoMode(900, 900),
                "oui"
            )
        )
    );

    engine.renderThreadEntry();
    engine.getRenderThread().wait();

    std::cin.get();

    /*std::cout << "c'est la fin" << std::endl;
    std::cin.get();*/

    /*
    sf::Thread client_thread(&client);
    sf::Thread server_thread(&server);

    server_thread.launch();
    client_thread.launch();

    server_thread.wait();
    std::cout << "Fin du thread serveur" << std::endl;

    client_thread.wait();
    std::cout << "Fin du thread client" << std::endl;

    system("PAUSE");
    return 0;*/
}

void client()
{
    /*

    std::cout << "Client : " << "Salutation je suis un client" << std::endl;

    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("127.0.0.1", 25565);

    if (status != sf::Socket::Done)
    {
        std::cout << "Client : " << "Erreur lors de la tentative de connection au serveur" << std::endl;
        return;
    }

    std::cout << "Client : " << "Connection avec le serveur etablie" << std::endl;


    sf::Packet packet;
    if(socket.receive(packet) != sf::Socket::Done)
    {
        std::cout << "Client : " << "Erreur lors de la réception du packet" << std::endl;
    }

    unsigned short a;
    std::string b;
    if(packet >> a&& packet >> b)
    {
        std::cout << "Client : " << "recived " << a << std::endl;
        std::cout << "Client : " << "recived " << b << std::endl;
    }
    else
    {
        std::cout << "Client : " << "erreur lors de la reception du packet" << std::endl;
    }

    while (true)
    {

    }*/
}

void server()
{
    /*std::cout << "Serveur : " << "Salutation je suis le serveur" << std::endl;

    int port = 25565;

    sf::TcpListener listener;
    
    if (listener.listen(port) != sf::Socket::Done)
    {
        std::cout << "Serveur : " << "Erreur lors du bind du port" << std::endl;
        return;
    }

    std::cout << "Serveur : " << "Le bind du port " << port << " est reussi" << std::endl;

    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Done)
    {
        std::cout << "Serveur : " << "Erreur lors de l'acceptation du client" << std::endl;
        return;
    }

    std::cout << "Serveur : " << "le client " << client.getRemoteAddress() << " est connecte" << std::endl;

    sf::Packet packet;
    packet << (unsigned short)10;
    packet << "oue";

    if (client.send(packet) != sf::Socket::Done)
    {
        std::cout << "Serveur : " << "Erreur lors de l'envoie du packet" << std::endl;
    }

    while (true)
    {

    }*/
}