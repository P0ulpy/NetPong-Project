#pragma once

#include "SFML/Graphics.hpp"

constexpr float PI = 3.14159265359f;

class Utils
{
public:
	//----------- MATHS -----------
	static float dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
	static sf::Vector2f normalize(const sf::Vector2f& originalVector);
	static double deceleration(const float initial, const float target, const float time);
	static float getDistance(float x1, float y1, float x2, float y2);
	static float getDistance(sf::Vector2f v1, sf::Vector2f v2);
	static sf::Vector2f getVectorReflection(const sf::Vector2f& inDirection, const sf::Vector2f& surfaceVector);
	static double degreeToRadian(double degree);
    static float clamp01(float x);
    static double lerp(float a, float b, float t);

	//----------- COLLISIONS -----------
	static bool circleCircleCollision(float c1x, float c1y, float c1r, float c2x, float c2y, float c2r);
	static bool lineLineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, sf::Vector2f& outIntersectionPoint);
	static bool pointCircleCollision(float px, float py, float cx, float cy, float r);
	static bool linePointCollision(float x1, float y1, float x2, float y2, float px, float py);
	static bool lineCircleCollision(float x1, float y1, float x2, float y2, float cX, float cY, float cR, sf::Vector2f& outImpactPoint);
};