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

	enum DrawDirection
	{
		TOP_LEFT_TO_BOTTOM_RIGHT = 0,
		TOP_RIGHT_TO_BOTTOM_LEFT,
		BOTTOM_RIGHT_TO_TOP_LEFT,
		BOTTOM_LEFT_TO_TOP_RIGHT
	};

	enum Orientation
	{
		ZERO = 0,
		FIFTEEN = 15,
		THIRTY = 30,
		FOURTY_FIVE = 45,
		SIXTY = 60,
		SEVENTY_FIVE = 75,
		NINETY = 90
	};

	sf::ConvexShape _terrainShape;
	sf::RectangleShape* _globalBounds;
	sf::Rect<float> _playableArea;

	sf::Vector2f _terrainSize{ 600, 600 };

	std::vector<sf::Vector2f> _pointPositions;

	void initShape(const sf::RenderWindow& window);
	void initPlayableArea();
	void initEdgesRegistration();

	sf::Vector2f initPoint(const sf::Vector2f& previousPoint, DrawDirection drawDirection, Orientation orientation, int pointDistance);
	void setPointAndUpdateCurrentPoint( DrawDirection drawDirection, Orientation orientation, int pointDistance, int& currentPointDrawned);
	sf::Vector2f normalize(const sf::Vector2f& originalVector);
	void drawTerrain1();
	void drawTerrain2();
	void drawTerrain3();
	void drawTerrain4();
	void drawTerrain5();
};