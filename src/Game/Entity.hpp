#pragma once

#include "SFML/Graphics.hpp"
#include "../Engine/Engine.hpp"


class Entity
{
protected:
	//PoPossibEngin& _engine = PoPossibEngin::getInstance();

	std::unique_ptr<sf::Texture> _texture;
	std::unique_ptr<sf::Sprite> _sprite;

	sf::Vector2f _velocity;
	float _currentSpeed { 200 };
	float _initialSpeed { 200 };
	float _maxSpeed { 500 };

public:
	Entity() = default;
	virtual ~Entity() = default;

	virtual void update(const float& deltaTime) = 0;
	virtual void render(sf::RenderTarget& renderTarget) const = 0;
	virtual void moveEntity(const sf::Vector2f& velocity, const float& deltaTime) = 0;

	void createSprite(const sf::Texture& pTexture);
};