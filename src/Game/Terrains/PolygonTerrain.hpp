#pragma once

#include "SFML/Graphics.hpp"

class Character;
class PongBall;

class PolygonTerrain
{
public:
	PolygonTerrain(const sf::RenderWindow& window, const std::vector<Character*>& players);
	virtual ~PolygonTerrain();

	//Functions
	void update(const float& deltaTime);
	void updateCollision(const float& deltaTime) const;
	void render(sf::RenderTarget& target) const;

	void drawRandomTerrain();

	//Gets - Sets
	sf::Vector2f getPointPosition(int index) const;
	sf::ConvexShape getShape() const;

    void addPongBall(PongBall* pongBall = nullptr);
    std::vector<PongBall*>& getPongBalls();
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
		FORTY_FIVE = 45,
		SIXTY = 60,
		SEVENTY_FIVE = 75,
		NINETY = 90
	};

	const sf::RenderWindow* _window;

	std::vector<PongBall*> _pongBalls;
	const std::vector<Character*>& _players;

	int _currentTerrain{ 1 };
	sf::ConvexShape _terrainShape;
	sf::Vector2f _firstPointPositionOfShape;
	std::vector<sf::Vector2f> _pointPositions;

	//Initializers
	void initShape();
	void initTerrainOrigin();
	void initEdgesRegistration();
	sf::Vector2f initPoint(const sf::Vector2f& previousPoint, DrawDirection drawDirection, Orientation orientation, int pointDistance);

	//Collision methods
	void checkPongBallTerrainCollision(const float& deltaTime) const;
	void checkPlayerTerrainCollision() const;

	//Gets - Sets
	void setPointAndUpdateCurrentPoint( DrawDirection drawDirection, Orientation orientation, int pointDistance, int& currentPointDrawned);

	//Drawing terrain
	void selectAndDrawRandomTerrain();

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
