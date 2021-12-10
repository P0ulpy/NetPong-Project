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

	sf::Vector2f getVectorReflection(sf::Vector2f inDirection, sf::Vector2f surfaceVector) const;
	sf::Vector2f getPointPosition(int index) const;

private:
	sf::ConvexShape _terrainShape;
	sf::Rect<float> _playableArea;

	sf::Vector2f _terrainSize{ 600, 600 };

	std::vector<sf::Vector2f> _pointPositions;

	void initShape(const sf::RenderWindow& window);
	void initPlayableArea();
	void initEdgesRegistration();
};