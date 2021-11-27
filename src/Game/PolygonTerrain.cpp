#include "PolygonTerrain.hpp"

#include <iostream>
#include <math.h>

PolygonTerrain::PolygonTerrain(const sf::RenderWindow& window)
{
	initShape(window);
	initPlayableArea();
}

PolygonTerrain::~PolygonTerrain()
{

}

void PolygonTerrain::update()
{

}

void PolygonTerrain::render(sf::RenderTarget& target) const
{
	target.draw(_terrainShape);
	target.draw(_proutctangle);
}

sf::ConvexShape PolygonTerrain::getShape() const
{
	return _terrainShape;
}

sf::Rect<float> PolygonTerrain::getPlayableArea() const
{
	return _playableArea;
}

void PolygonTerrain::initShape(const sf::RenderWindow& window)
{
	float _terrainXsize = 500.f;
	float _terrainYsize = 700.f;

	float _terrainAngleXSize = 50.f;
	float _terrainAngleYSize = 150.f;

	_terrainShape.setFillColor(sf::Color::Black);
	_terrainShape.setOutlineColor(sf::Color::White);
	_terrainShape.setOutlineThickness(10.f);
	_terrainShape.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - _terrainXsize / 2.f, static_cast<float>(window.getSize().y) / 2.f - _terrainYsize / 2.f));

	_terrainShape.setPointCount(8);

	_terrainShape.setPoint(0, sf::Vector2f(_terrainAngleXSize, 0));
	_terrainShape.setPoint(1, sf::Vector2f(_terrainXsize - _terrainAngleXSize, 0));
	_terrainShape.setPoint(2, sf::Vector2f(_terrainXsize, _terrainAngleYSize));
	_terrainShape.setPoint(3, sf::Vector2f(_terrainXsize, _terrainYsize - _terrainAngleYSize));
	_terrainShape.setPoint(4, sf::Vector2f(_terrainXsize - _terrainAngleXSize, _terrainYsize));
	_terrainShape.setPoint(5, sf::Vector2f(_terrainAngleXSize, _terrainYsize));
	_terrainShape.setPoint(6, sf::Vector2f(0, _terrainYsize - _terrainAngleYSize));
	_terrainShape.setPoint(7, sf::Vector2f(0, _terrainAngleYSize));

	// ------------------------------------------------------------
	_proutctangle.setSize(sf::Vector2f(150, 1));
	_proutctangle.setOutlineThickness(5);
	_proutctangle.setOrigin(_proutctangle.getSize().x / 2.f, _proutctangle.getSize().y / 2.f);
	_proutctangle.setPosition(600,300);
	_proutctangle.rotate(30);
}

void PolygonTerrain::initPlayableArea()
{
	_playableArea = sf::Rect(_terrainShape.getGlobalBounds().left + _terrainShape.getOutlineThickness(),
		_terrainShape.getGlobalBounds().top + _terrainShape.getOutlineThickness(),
		_terrainShape.getGlobalBounds().left + _terrainShape.getGlobalBounds().width - _terrainShape.getOutlineThickness(),
		_terrainShape.getGlobalBounds().top + _terrainShape.getGlobalBounds().height - _terrainShape.getOutlineThickness());
}

sf::Vector2f PolygonTerrain::getVectorReflection(sf::Vector2f inDirection, sf::Vector2f surfaceVector)
{
	sf::Vector2 inNormal = sf::Vector2f(-surfaceVector.y, surfaceVector.x);
	float factor = -2.f * (inDirection.x * inNormal.x + inDirection.y * inNormal.y);
	sf::Vector2f finalVector = sf::Vector2(std::round(factor * inNormal.x + inDirection.x),
											std::round(factor * inNormal.y + inDirection.y));
	return finalVector;
}

sf::Vector2f PolygonTerrain::getVectorReflection(sf::Vector2f inDirection, float surfaceAngle)
{
	sf::Vector2f surfaceVector = sf::Vector2f(std::cosf(surfaceAngle), std::sinf(surfaceAngle));
	surfaceVector.x = std::atan2f(surfaceVector.x, surfaceVector.y);
	sf::Vector2 inNormal = sf::Vector2f(-surfaceVector.y, surfaceVector.x);
	float factor = -2.f * (inDirection.x * inNormal.x + inDirection.y * inNormal.y);
	sf::Vector2f finalVector = sf::Vector2(factor * inNormal.x + inDirection.x, factor * inNormal.y + inDirection.y);
	return finalVector;
}

