#pragma once

#include "SFML/Graphics.hpp"

class Entity
{
protected:
	std::unique_ptr<sf::Texture> _texture;
	std::unique_ptr<sf::Sprite> _sprite;

	sf::Vector2f _velocity;
	float _currentSpeed;
	float _initialSpeed;
	float _maxSpeed;

public:
	Entity();
	virtual ~Entity();

	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget& renderTarget) const = 0;

	virtual void moveEntity(const sf::Vector2f& velocity, const float& deltaTime) = 0;

	void createSprite(const sf::Texture& pTexture);
};