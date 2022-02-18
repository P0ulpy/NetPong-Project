#include "PolygonTerrain.hpp"

#include <iostream>
#include "../../Utils/Utils.hpp"
#include "../Entities/PongBall.hpp"
#include "../Entities/Character.hpp"
#include <cmath>

constexpr int TERRAIN_SIZE_MULTIPLIER = 20;
constexpr int TERRAIN_OUTLINE_THICKNESS = 20;
constexpr int TERRAIN_NUM_EDGES = 8;

constexpr int NUM_TERRAINS = 10;

PolygonTerrain::PolygonTerrain(const sf::RenderWindow& window, const std::vector<PongBall*>& pongBall, const std::vector<Character*>& players)
	:_window(&window), _pongBalls(pongBall), _players(players)
{
	initShape();
}

PolygonTerrain::~PolygonTerrain() = default;

void PolygonTerrain::update(const float& deltaTime)
{
	updateCollision(deltaTime);
}

void PolygonTerrain::updateCollision(const float& deltaTime) const
{
	checkPongBallTerrainCollision(deltaTime);
	checkPlayerTerrainCollision();

	//A CHANGER
	for (const auto pongBall : _pongBalls)
	{
		if(pongBall->isActive())
			pongBall->resetBallDestAndOldPos(deltaTime);
	}
}

void PolygonTerrain::render(sf::RenderTarget& target) const
{
	target.draw(_terrainShape);
}

void PolygonTerrain::checkPongBallTerrainCollision(const float& deltaTime) const
{
	float remainingTime = 1.f;
	const int pointCount = static_cast<int>(getShape().getPointCount());
	bool hasCollisionOccurred;

	//For every side of the terrain
	do
	{
		hasCollisionOccurred = false;

		for (int i = 0; i < pointCount; i++)
		{
			const float xA = getPointPosition(i).x;
			const float yA = getPointPosition(i).y;
			const float xB = getPointPosition((i + 1) % pointCount).x;
			const float yB = getPointPosition((i + 1) % pointCount).y;

			for (const auto pongBall : _pongBalls)
			{
				if (pongBall->isActive() && pongBall->hitWallIfCollision(xA, yA, xB, yB, remainingTime, deltaTime))
				{
					pongBall->addNumBounceAndUpdateVisibility();
					hasCollisionOccurred = true;
					break;
				}
			}

			if (hasCollisionOccurred) break;
		}
	} while (hasCollisionOccurred && remainingTime > 0.f);
}

void PolygonTerrain::checkPlayerTerrainCollision() const
{
	const int pointCount = static_cast<int>(getShape().getPointCount());

	for (int i = 0; i < pointCount; i++)
	{
		const float xA = getPointPosition(i).x;
		const float yA = getPointPosition(i).y;
		const float xB = getPointPosition((i + 1) % pointCount).x;
		const float yB = getPointPosition((i + 1) % pointCount).y;

		for (const auto player : _players)
		{
			if(player->canCharacterMove())
			{
				player->hitWallIfCollision(xA, yA, xB, yB);
			}
		}
	}
}

sf::ConvexShape PolygonTerrain::getShape() const
{
	return _terrainShape;
}

void PolygonTerrain::initShape()
{
	_terrainShape.setFillColor(sf::Color::Black);
	_terrainShape.setOutlineColor(sf::Color(255,65,225,255));
	_terrainShape.setOutlineThickness(TERRAIN_OUTLINE_THICKNESS);

	drawRandomTerrain();
}

void PolygonTerrain::initEdgesRegistration()
{
	_pointPositions.clear();

	const int pointCount = static_cast<int>(getShape().getPointCount());

	for (int i = 0; i < pointCount; i++)
	{
		const sf::Vector2 transformPoint{ _terrainShape.getPoint(i).x, _terrainShape.getPoint(i).y };
		
		_pointPositions.emplace_back(
			_terrainShape.getTransform().transformPoint(transformPoint).x,
			_terrainShape.getTransform().transformPoint(transformPoint).y
		);
	}
}

sf::Vector2f PolygonTerrain::initPoint(const sf::Vector2f& previousPoint, DrawDirection drawDirection,
	Orientation orientationDraw, int pointDistance = 1)
{
	float directionX = 1;
	float directionY = 1;

	switch (drawDirection)
	{
		case TOP_LEFT_TO_BOTTOM_RIGHT: directionX = directionY = 1; break;
		case TOP_RIGHT_TO_BOTTOM_LEFT: directionX = -1; directionY = 1; break;
		case BOTTOM_RIGHT_TO_TOP_LEFT: directionX = directionY = -1; break;
		case BOTTOM_LEFT_TO_TOP_RIGHT: directionX = 1; directionY = -1; break;
	}

	sf::Vector2f newPoint = sf::Vector2f(
		static_cast<float>(std::cos(Utils::degreeToRadian(orientationDraw))), 
		static_cast<float>(std::sin(Utils::degreeToRadian(orientationDraw)))
	);

	newPoint.x = previousPoint.x + directionX * newPoint.x * static_cast<float>(pointDistance) * TERRAIN_SIZE_MULTIPLIER;
	newPoint.y = previousPoint.y + directionY * newPoint.y * static_cast<float>(pointDistance) * TERRAIN_SIZE_MULTIPLIER;

	return newPoint;
}

void PolygonTerrain::initTerrainOrigin()
{
	float topX = getPointPosition(0).x;
	float topY = getPointPosition(0).y;
	const float lineThickness = _terrainShape.getOutlineThickness();

	//Trying to get the highest X and leftmost Y
	for (int i = 0; i < getShape().getPointCount(); i++)
	{
		topX = std::min(topX, getPointPosition(i).x);
		topY = std::min(topY, getPointPosition(i).y);
	}

	_terrainShape.setOrigin(sf::Vector2f(
		-static_cast<float>(_window->getSize().x) / 2.f + _terrainShape.getGlobalBounds().width / 2.f + topX - lineThickness,
		-static_cast<float>(_window->getSize().y) / 2.f + _terrainShape.getGlobalBounds().height / 2.f + topY - lineThickness)
	);

	initEdgesRegistration();//We register again the edges so the points positions update with their new origin.
}

void PolygonTerrain::setPointAndUpdateCurrentPoint(DrawDirection drawDirection, Orientation orientation, int pointDistance, int& currentPointDrawned)
{
	_terrainShape.setPoint(currentPointDrawned, initPoint(_terrainShape.getPoint(currentPointDrawned - 1), drawDirection, orientation, pointDistance));
	currentPointDrawned++;
}


sf::Vector2f PolygonTerrain::getPointPosition(const int index) const
{
	return _pointPositions.at(index);
}

void PolygonTerrain::drawRandomTerrain()
{
	_terrainShape.setOrigin(sf::Vector2f(0, 0));

	selectAndDrawRandomTerrain();

	initEdgesRegistration();
	initTerrainOrigin();
}

void PolygonTerrain::selectAndDrawRandomTerrain()
{
	//Pick random index between 1 and NUM_TERRAINS, that is different from the previous one
	srand(static_cast<unsigned>(time(nullptr)));
	int randomTerrainIndex;
	do
	{
		randomTerrainIndex = rand() % NUM_TERRAINS + 1;
	} while (randomTerrainIndex == _currentTerrain);

	_currentTerrain = randomTerrainIndex;

	//Draw random terrain
	switch (randomTerrainIndex)
	{
		case 1: drawTerrain1(); break;
		case 2: drawTerrain2(); break;
		case 3: drawTerrain3(); break;
		case 4: drawTerrain4(); break;
		case 5: drawTerrain5(); break;
		case 6: drawTerrain6(); break;
		case 7: drawTerrain7(); break;
		case 8: drawTerrain8(); break;
		case 9: drawTerrain9(); break;
		case 10:drawTerrain10();break;
		default:drawTerrain1(); break;
	}
}

//=========================== DRAWING TERRAINS ===========================

void PolygonTerrain::drawTerrain1()//OCTOGON
{
	int edgeLenght = 16;

	_terrainShape.setPointCount(TERRAIN_NUM_EDGES);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FORTY_FIVE, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, NINETY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FORTY_FIVE, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, FORTY_FIVE, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, NINETY, edgeLenght, currentPoint);

}
void PolygonTerrain::drawTerrain2()//CARRE ORIENTE
{
	int edgeLenght = 28;

	_terrainShape.setPointCount(4);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FIFTEEN, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, SEVENTY_FIVE, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, FIFTEEN, edgeLenght, currentPoint);
}

void PolygonTerrain::drawTerrain3()//CARRE AVEC ANGLES LEGERS
{
	int edgeLenght = 24;

	_terrainShape.setPointCount(8);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FORTY_FIVE, edgeLenght / 3, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, NINETY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FORTY_FIVE, edgeLenght / 3, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint( BOTTOM_RIGHT_TO_TOP_LEFT, FORTY_FIVE, edgeLenght / 3, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, NINETY, edgeLenght, currentPoint);
}

void PolygonTerrain::drawTerrain4()//TREFLE
{
	int edgeLenght = 8;
	int edgeLenght2 = edgeLenght / 2;
	int edgeLenght4 = edgeLenght / 3;

	_terrainShape.setPointCount(24);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, ZERO, edgeLenght4, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_LEFT_TO_TOP_RIGHT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FORTY_FIVE, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, NINETY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, NINETY, edgeLenght4, currentPoint);

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, NINETY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FORTY_FIVE, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, ZERO, edgeLenght4, currentPoint);

	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, FORTY_FIVE, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, NINETY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_LEFT_TO_TOP_RIGHT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_LEFT_TO_TOP_RIGHT, NINETY, edgeLenght4, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_LEFT_TO_TOP_RIGHT, NINETY, edgeLenght, currentPoint);
}

void PolygonTerrain::drawTerrain5() //CARRE AVEC ANGLES CREUSES
{
	int edgeLenght = 20;
	int edgeLenght2 = edgeLenght / 3;

	_terrainShape.setPointCount(12);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, SEVENTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FIFTEEN, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, NINETY, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FIFTEEN, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, SEVENTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, ZERO, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, SEVENTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, FIFTEEN, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, NINETY, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_LEFT_TO_TOP_RIGHT, FIFTEEN, edgeLenght2, currentPoint);
}

void PolygonTerrain::drawTerrain6() //OCTOGON ORIENT�
{
	int edgeLenght = 16;
	int edgeLenght2 = 12;

	_terrainShape.setPointCount(8);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, THIRTY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, SEVENTY_FIVE, edgeLenght2, currentPoint);

	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, SIXTY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FIFTEEN, edgeLenght2, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, THIRTY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, SEVENTY_FIVE, edgeLenght2, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_LEFT_TO_TOP_RIGHT, SIXTY, edgeLenght, currentPoint);
}

void PolygonTerrain::drawTerrain7() //HEXAGON
{
	int edgeLenght = 20;

	_terrainShape.setPointCount(6);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, THIRTY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, NINETY, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, THIRTY, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, THIRTY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, NINETY, edgeLenght, currentPoint);
}

void PolygonTerrain::drawTerrain8() //HEXAGON PENCHE
{
	int edgeLenght = 20;

	_terrainShape.setPointCount(6);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FIFTEEN, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, SEVENTY_FIVE, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FORTY_FIVE, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, FIFTEEN, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, SEVENTY_FIVE, edgeLenght, currentPoint);
}

void PolygonTerrain::drawTerrain9() //CARRE AVEC ANGLES CREUSES ORIENTE
{
	int edgeLenght = 26;
	int edgeLenght2 = edgeLenght / 2;
	int edgeLenght3 = edgeLenght / 3;

	_terrainShape.setPointCount(8);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FORTY_FIVE, edgeLenght3, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, NINETY, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FORTY_FIVE, edgeLenght3, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, FORTY_FIVE, edgeLenght3, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, NINETY, edgeLenght2, currentPoint);
}

void PolygonTerrain::drawTerrain10() //PLUS
{
	int edgeLenght = 6;
	int edgeLenght2 = 12;

	_terrainShape.setPointCount(16);
	_terrainShape.setPoint(0, sf::Vector2f(0,0));
	int currentPoint = 1;

	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, ZERO, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, NINETY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_LEFT_TO_BOTTOM_RIGHT, ZERO, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, NINETY, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(TOP_RIGHT_TO_BOTTOM_LEFT, NINETY, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, ZERO, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, NINETY, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, FORTY_FIVE, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_RIGHT_TO_TOP_LEFT, ZERO, edgeLenght, currentPoint);

	setPointAndUpdateCurrentPoint(BOTTOM_LEFT_TO_TOP_RIGHT, NINETY, edgeLenght2, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_LEFT_TO_TOP_RIGHT, ZERO, edgeLenght, currentPoint);
	setPointAndUpdateCurrentPoint(BOTTOM_LEFT_TO_TOP_RIGHT, FORTY_FIVE, edgeLenght2, currentPoint);
}