#pragma once

#include <iostream>
#include <vector>
#include <stack>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"

class Scene
{
private:

protected:
	std::unique_ptr<sf::RenderWindow*> _window;
	std::vector<sf::Texture> _textures;

public:
	Scene(sf::RenderWindow* window);
	virtual ~Scene();

	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};
