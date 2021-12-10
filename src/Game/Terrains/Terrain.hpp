#pragma once

#include "SFML/Graphics.hpp"

class MainGameScene;

class Terrain
{
public:
	Terrain(const sf::RenderWindow& window);
	virtual ~Terrain();

	//Functions
	void update();
	void render(sf::RenderTarget& target) const;

	sf::RectangleShape getShape() const;
	sf::Rect<float> getPlayableArea() const;

private:
	sf::RectangleShape _terrainShape;
	sf::Rect<float> _playableArea;

	sf::Vector2f _terrainSize{ 500, 700 };

	void initShape(const sf::RenderWindow& window);
	void initPlayableArea();
};