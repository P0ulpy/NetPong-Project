#pragma once

#include "SFML/Graphics.hpp"

class PolygonCollisionResult
{
public:
	PolygonCollisionResult();
	~PolygonCollisionResult();
	// Are the polygons going to intersect forward in time?
	bool WillIntersect { false };
	// Are the polygons currently intersecting?
	bool Intersect { false };
	// The translation to apply to the first polygon to push the polygons apart.
	sf::Vector2f MinimumTranslationVector {0,0};

	void ProjectPolygon(sf::Vector2f axis, sf::ConvexShape polygon, float& min, float& max);
	void ProjectPolygon(sf::Vector2f axis, sf::CircleShape ballPolygon, float& min, float& max);
	void ProjectPolygon(sf::Vector2f axis, sf::RectangleShape rectPolygon, float& min, float& max);
	float IntervalDistance(float minA, float maxA, float minB, float maxB);
	PolygonCollisionResult PolygonCollision(sf::CircleShape polygonBall, sf::RectangleShape polygonRect, sf::Vector2f velocity);
};