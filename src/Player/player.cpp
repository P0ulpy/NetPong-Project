#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>    
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "player.h"

#include <math.h>

player::player() {
    sf::RenderWindow window(sf::VideoMode(1200, 900), "PONG");
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    sf::Texture texture;
    sf::Texture bouclier;

    if (!texture.loadFromFile("./Assets/Rond_rouge.png"))

    {

        std::cerr << "Error while loading texture" << std::endl;

        

    }



    if (!bouclier.loadFromFile("./Assets/Barre.jpg"))

    {

        std::cerr << "Error while loading bouclier texture" << std::endl;

        

    }
    // Enable the smooth filter. The texture appears smoother so that pixels are less noticeable.
    texture.setSmooth(true);
    bouclier.setSmooth(true);

    // Create the sprite and apply the texture

    sf::Sprite sprite;
    sf::Sprite bouclierSPRITE;

    sprite.setTexture(texture);
    sprite.setScale(0.2, 0.2);

    bouclierSPRITE.setTexture(bouclier);
    bouclierSPRITE.setScale(1, 1);

    sf::FloatRect spriteSize = sprite.getGlobalBounds();
    sf::FloatRect bouclierSize = bouclierSPRITE.getGlobalBounds();


    sprite.setOrigin((spriteSize.width / 2) / sprite.getScale().x, (spriteSize.height / 2) / sprite.getScale().y);
    bouclierSPRITE.setOrigin((bouclierSize.width / 2) / bouclierSPRITE.getScale().x, (bouclierSize.height / 2) / bouclierSPRITE.getScale().y);

}

player::~player() {

}

void Character(int speed) {
    // Sprite coordinates
    int x = window.getSize().x / 2.;
    int xb = window.getSize().x / 2.;

    int y = window.getSize().y / 1.2;
    int yb = window.getSize().y / 1.2;
    
    bool leftFlag = false;
    bool rightFlag = false;
    
    while (window.isOpen())

    {

        // Process events

        sf::Event event;

        while (window.pollEvent(event))

        {


            // Close the window if a key is pressed or if requested

            if (event.type == sf::Event::Closed) window.close();



            // If a key is pressed

            if (event.type == sf::Event::KeyPressed)

            {

                switch (event.key.code)

                {

                    // If escape is pressed, close the application

                case  sf::Keyboard::Escape: window.close(); break;



                    // Process the up, down, left and right keys

                    //  case sf::Keyboard::Up:     upFlag = true; break;

                    //  case sf::Keyboard::Down:    downFlag = true; break;

                case sf::Keyboard::Left:    leftFlag = true; break;

                case sf::Keyboard::Right:   rightFlag = true; break;

                default: break;

                }

            }



            // If a key is released

            if (event.type == sf::Event::KeyReleased)

            {

                switch (event.key.code)

                {

                    // Process the up, down, left and right keys

           //     case sf::Keyboard::Up:     upFlag = false; break;

             //   case sf::Keyboard::Down:    downFlag = false; break;

                case sf::Keyboard::Left:    leftFlag = false; break;

                case sf::Keyboard::Right:   rightFlag = false; break;

                default: break;

                }

            }

        }



        // Update coordinates

        if (leftFlag) x -= speed;
        if (leftFlag) xb -= speed;

        if (rightFlag) x += speed;
        if (rightFlag) xb += speed;

        //   if (upFlag) y -= SPRITE_SPEED;

      //  if (downFlag) y += SPRITE_SPEED;



        // Check screen boundaries

        if (x < 0) x = 0;

        if (x > (int)window.getSize().x) x = window.getSize().x;

        if (xb < 0) xb = 0;

        if (xb > (int)window.getSize().x) xb = window.getSize().x;
        // if (y < 0) y = 0;

       //  if (y > (int)window.getSize().y) y = window.getSize().y;



         // Clear the window and apply grey background

        window.clear(sf::Color(127, 127, 127));



        // Rotate and draw the sprite
        //Calcul angle bouclier et personnage
        sf::Vector2f curPos = sprite.getPosition();
        sf::Vector2i position = sf::Mouse::getPosition(window);

        float dx = curPos.x - position.x;
        float dy = curPos.y - position.y;


        float rotation = ((atan2(dy, dx)) * 180.0 / PI) - 90;

        sprite.setRotation(rotation);
        bouclierSPRITE.setRotation(rotation);


        sf::Vector2f p = sprite.getTransform().transformPoint(sf::Vector2f(sprite.getOrigin().x, sprite.getOrigin().y - (spriteSize.width) * 2.5));


        sprite.setPosition(x, y);
        window.draw(sprite);

        bouclierSPRITE.setPosition(p.x, p.y);
        window.draw(bouclierSPRITE);

        // Update display and wait for vsync

        window.display();

    }
}