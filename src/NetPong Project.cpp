#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>    
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

int main()
{
    system("echo %cd%");

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    // Load a sprite to display
    sf::Texture texture;
    texture.loadFromFile("./Assets/cute_image.jpg", sf::IntRect(400, 800, 800, 600));

    sf::Sprite sprite(texture);
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("./Assets/arial.ttf"))
        return EXIT_FAILURE;
    sf::Text text("CC UWU", font, 50);
    text.setPosition(100, 100);
    text.setFillColor(sf::Color(255, 50, 50));

    //window.draw(text);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        // Draw the sprite
        window.draw(sprite);
        // Draw the string
        window.draw(text);
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}