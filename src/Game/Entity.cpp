#include "Entity.hpp"

Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::createSprite(const sf::Texture& pTexture)
{
	_texture = std::make_unique<sf::Texture>(pTexture);
}

