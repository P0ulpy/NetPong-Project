#pragma once

#include "SFML/Graphics.hpp"

class Character;
class PongBall;

class PolygonTerrain
{
public:
	PolygonTerrain(const sf::RenderWindow& window, const std::vector<PongBall*>& pongBalls, const std::vector<Character*>& players);
	virtual ~PolygonTerrain();

	//Functions
	void update(const float& deltaTime);
	void updateCollision(const float& deltaTime) const;
	void render(sf::RenderTarget& target) const;

	sf::ConvexShape getShape() const;
	sf::Rect<float> getPlayableArea() const;

	sf::Vector2f getPointPosition(int index) const;

	void drawRandomTerrain();

private:

	enum DrawDirection
	{
		TOP_LEFT_TO_BOTTOM_RIGHT,
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

	const sf::RenderWindow* _window;

	const std::vector<PongBall*>& _pongBalls;
	const std::vector<Character*>& _players;

	sf::ConvexShape _terrainShape;
	sf::Rect<float> _playableArea;

	sf::Vector2f _terrainSize{ 600, 600 };

	std::vector<sf::Vector2f> _pointPositions;

	int _tempCurrentTerrain {1};

	void initShape();
	void initPlayableArea();
	void initEdgesRegistration();

	sf::Vector2f initPoint(const sf::Vector2f& previousPoint, DrawDirection drawDirection, Orientation orientation, int pointDistance);
	void setPointAndUpdateCurrentPoint( DrawDirection drawDirection, Orientation orientation, int pointDistance, int& currentPointDrawned);
	void selectAndDrawRandomTerrain();
	void initTerrainOrigin();

	void drawTerrain1();
	void drawTerrain2();
	void drawTerrain3();
	void drawTerrain4();
	void drawTerrain5();
	void drawTerrain6();
	void drawTerrain7();
	void drawTerrain8();
	void drawTerrain9();
	void drawTerrain10();
};
