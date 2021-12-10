#include "Terrain.hpp"

Terrain::Terrain(const sf::RenderWindow& window)
{
	initShape(window);
	initPlayableArea();
}

Terrain::~Terrain()
{

}

void Terrain::update()
{

}

void Terrain::render(sf::RenderTarget& target) const
{
	target.draw(_terrainShape);
}

sf::RectangleShape Terrain::getShape() const
{
	return _terrainShape;
}

sf::Rect<float> Terrain::getPlayableArea() const
{
	return _playableArea;
}

void Terrain::initShape(const sf::RenderWindow& window)
{
	_terrainShape.setSize(_terrainSize);
	_terrainShape.setFillColor(sf::Color::Black);
	_terrainShape.setOutlineColor(sf::Color::White);
	_terrainShape.setOutlineThickness(10);
	_terrainShape.setPosition(sf::Vector2f(window.getSize().x / 2 - _terrainSize.x / 2,
		window.getSize().y / 2 - _terrainSize.y / 2));
}

void Terrain::initPlayableArea()
{
	_playableArea = sf::Rect(_terrainShape.getGlobalBounds().left + _terrainShape.getOutlineThickness(),
		_terrainShape.getGlobalBounds().top + _terrainShape.getOutlineThickness(),
		_terrainShape.getGlobalBounds().left + _terrainShape.getGlobalBounds().width - _terrainShape.getOutlineThickness(),
		_terrainShape.getGlobalBounds().top + _terrainShape.getGlobalBounds().height - _terrainShape.getOutlineThickness());
}