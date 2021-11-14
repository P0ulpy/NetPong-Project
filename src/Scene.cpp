#include "Scene.h"

Scene::Scene(sf::RenderWindow* window)
{
	_window = std::make_unique<sf::RenderWindow*>(window);
}

Scene::~Scene()
{

}
