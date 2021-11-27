#pragma once

#include "SFML/Graphics.hpp"

class PolygonTerrain
{
public:
	PolygonTerrain(const sf::RenderWindow& window);
	virtual ~PolygonTerrain();

	//Functions
	void update();
	void render(sf::RenderTarget& target) const;

	sf::ConvexShape getShape() const;
	sf::Rect<float> getPlayableArea() const;

	sf::RectangleShape _proutctangle;

	sf::Vector2f getVectorReflection(sf::Vector2f inDirection, sf::Vector2f surfaceVector);
	sf::Vector2f getVectorReflection(sf::Vector2f originalVector, float surfaceAngle);
private:
	sf::ConvexShape _terrainShape;
	sf::Rect<float> _playableArea;

	sf::Vector2f _terrainSize{ 500, 700 };

	void initShape(const sf::RenderWindow& window);
	void initPlayableArea();
};