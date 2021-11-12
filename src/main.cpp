/*#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>    
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include <math.h>

#define PI 3.14159265
#define         SPRITE_SPEED        5

int main()
{
    // _____________________

    // ::: Create window :::



    // Create a window with the same pixel depth as the desktop

    sf::RenderWindow window(sf::VideoMode(1200, 900), "PONG");
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("./Assets/arial.ttf"))
    {
        std::cerr << "Error while loading font" << std::endl;

        return -1;
    }
    

    // Enable vertical sync. (vsync)

    window.setVerticalSyncEnabled(true);

    // When a key is pressed, sf::Event::KeyPressed will be true only once

    window.setKeyRepeatEnabled(false);



    // ____________________

    // ::: Load texture :::



    // Create texture from PNG file

    sf::Texture texture;
    sf::Texture bouclier;

    if (!texture.loadFromFile("./Assets/Rond_rouge.png"))

    {

        std::cerr << "Error while loading texture" << std::endl;

        return -1;

    }

    if (!bouclier.loadFromFile("./Assets/Bouclier.jpg"))

    {

        std::cerr << "Error while loading bouclier texture" << std::endl;

        return -1;

    }

    // Enable the smooth filter. The texture appears smoother so that pixels are less noticeable.

    texture.setSmooth(true);
    bouclier.setSmooth(true);



    // _______________________________________

    // ::: Create sprite and apply texture :::



    // Create the sprite and apply the texture

    sf::Sprite sprite;
    sf::Sprite bouclierSPRITE;

    sprite.setTexture(texture);
    sprite.setScale(0.2,0.2);

    bouclierSPRITE.setTexture(bouclier);
    bouclierSPRITE.setScale(0.5,0.5);

    sf::FloatRect spriteSize = sprite.getGlobalBounds();
    sf::FloatRect bouclierSize = bouclierSPRITE.getGlobalBounds();

    sprite.setOrigin(spriteSize.width, -1500);
    bouclierSPRITE.setOrigin((spriteSize.width)-35, -550);




    // _________________

// ::: Main loop :::



// Sprite coordinates

    int x = window.getSize().x / 2.;
    int xb = window.getSize().x / 2.;

    int y = window.getSize().y / 2.;
    int yb = window.getSize().y / 2.;

  

    // Flags for key pressed

   // bool upFlag = false;

   // bool downFlag = false;

    bool leftFlag = false;

    bool rightFlag = false;



    sf::Clock timer;

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

        if (leftFlag) x -= SPRITE_SPEED;
        if (leftFlag) xb -= SPRITE_SPEED;

        if (rightFlag) x += SPRITE_SPEED;
        if (rightFlag) xb += SPRITE_SPEED;

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
        

      



        sprite.setPosition(x, y);
        bouclierSPRITE.setPosition(xb, yb);

        // Produit vectoriel
        float xm = sf::Mouse::getPosition().x;
        float ym = sf::Mouse::getPosition().y;
        float xbouclier = bouclierSPRITE.getPosition().x;
        float ybouclier = bouclierSPRITE.getPosition().y;


        float Na = sqrt(xm * xm + ym * ym);
        float Nb = sqrt(xbouclier * xbouclier + ybouclier * ybouclier);
        float C = (xm * xbouclier + ym * ybouclier)/(Na*Nb);
        float S = (xm * ybouclier - ym * xbouclier);

        int sign = 0;
        if (S > 0) sign = 1;
        if (S < 0) sign = -1;

        float angle = acos(C) * 180.0 / PI;

        text.setFont(font);
        text.setPosition(window.getSize().x/2, window.getSize().y/2);

        text.setString(std::to_string(angle));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        window.draw(text);
       // std::cout << xm << std::endl;;


       bouclierSPRITE.setRotation(angle);
      // sprite.setRotation(angle);
       
        window.draw(sprite);
        window.draw(bouclierSPRITE);
       
        

        // Update display and wait for vsync

        window.display();

    }

    return 0;

    return EXIT_SUCCESS;
}