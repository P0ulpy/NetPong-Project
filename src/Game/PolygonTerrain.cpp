#include "PolygonTerrain.hpp"

#include <iostream>
#include <math.h>

PolygonTerrain::PolygonTerrain(const sf::RenderWindow& window)
{
	initShape(window);
	initPlayableArea();
	initEdgesRegistration();
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
	const float _terrainXsize = 600.f;
	const float _terrainYsize = 600.f;

	const float _terrainAngleXSize = 100.f;
	const float _terrainAngleYSize = 175.f;

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
}

void PolygonTerrain::initPlayableArea()
{
	_playableArea = sf::Rect(_terrainShape.getGlobalBounds().left + _terrainShape.getOutlineThickness(),
		_terrainShape.getGlobalBounds().top + _terrainShape.getOutlineThickness(),
		_terrainShape.getGlobalBounds().left + _terrainShape.getGlobalBounds().width - _terrainShape.getOutlineThickness(),
		_terrainShape.getGlobalBounds().top + _terrainShape.getGlobalBounds().height - _terrainShape.getOutlineThickness());
}

void PolygonTerrain::initEdgesRegistration()
{
	const int pointCount = static_cast<int>(getShape().getPointCount());

	for (int i = 0; i < pointCount; i++)
	{
		const sf::Vector2 transformPoint{ _terrainShape.getPoint(i).x, _terrainShape.getPoint(i).y };
		
		_pointPositions.emplace_back(_terrainShape.getTransform().transformPoint(transformPoint).x,	
									 _terrainShape.getTransform().transformPoint(transformPoint).y);
	}
}

sf::Vector2f PolygonTerrain::getVectorReflection(sf::Vector2f inDirection, sf::Vector2f surfaceVector) const
{
	const auto inNormal = sf::Vector2f(-surfaceVector.y, surfaceVector.x);
	const float factor = -2.f * (inDirection.x * inNormal.x + inDirection.y * inNormal.y);
	const auto finalVector = sf::Vector2(std::round(factor * inNormal.x + inDirection.x),
										 std::round(factor * inNormal.y + inDirection.y));
	return finalVector;
}

sf::Vector2f PolygonTerrain::getPointPosition(const int index) const
{
	return _pointPositions.at(index);
}

