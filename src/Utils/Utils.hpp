#pragma once

#include "SFML/Graphics.hpp"

class Utils
{
public:
	static float dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
	static sf::Vector2f normalize(const sf::Vector2f& originalVector);

	static sf::Vector2f getVectorReflection(const sf::Vector2f& inDirection, const sf::Vector2f& surfaceVector);

	static float deceleration(float initial, float target, float time);
	static bool lineCircleCollision(float x1, float y1, float x2, float y2, float cx, float cy, float r, sf::Vector2f& outImpactPoint);
	static bool lineLineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, sf::Vector2f& outIntersectionPoint);
	static bool pointCircleCollision(float px, float py, float cx, float cy, float r);
	static bool linePointCollision(float x1, float y1, float x2, float y2, float px, float py);
	static float getDistance(float x1, float y1, float x2, float y2);
};