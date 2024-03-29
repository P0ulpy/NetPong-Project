#include "Utils.hpp"

//----------- MATHS -----------
float Utils::getDistance(float x1, float y1, float x2, float y2)
{
	const float distX = x1 - x2;
	const float distY = y1 - y2;
	return std::sqrt(distX*distX + distY * distY);
}

float Utils::getDistance(sf::Vector2f v1, sf::Vector2f v2)
{
	const float distX = v1.x - v2.x;
	const float distY = v1.y - v2.y;
	return std::sqrt(distX * distX + distY * distY);
}

double Utils::deceleration(const float initial, const float target, const float time)
{
	//Linear(Y0,Y1,t) = Y0 + t(Y1 - Y0)
	//Deceleration(Y0,Y1,t) = Linear( Y0, Y1, 1 - pow(1 - t,2) )
	//t = 1 - pow(1 - t,2), donc :
	//LinearDec(Y0,Y1,t) = Y0 + ( 1 - pow(1 - t,2) ) * (Y1 - Y0) )
	return static_cast<float>(initial) + (1 - std::pow(1 - time / 2, 2)) * (target - initial);
}

sf::Vector2f Utils::normalize(const sf::Vector2f& originalVector)
{
	const float norm = std::sqrt((originalVector.x * originalVector.x) + (originalVector.y * originalVector.y));

	// Prevent division by zero
	if (norm <= std::numeric_limits<float>::epsilon() * norm * 2
		|| norm < std::numeric_limits<float>::min())
	{
		return sf::Vector2f{};
	}

	return originalVector / norm;
}

float Utils::dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}


sf::Vector2f Utils::getVectorReflection(const sf::Vector2f& inDirection, const sf::Vector2f& surfaceVector)
{
	const auto inNormal = sf::Vector2f(-surfaceVector.y, surfaceVector.x);
	const float factor = -2.f * (inDirection.x * inNormal.x + inDirection.y * inNormal.y);
	const auto finalVector = sf::Vector2f(factor * inNormal.x + inDirection.x,
		factor * inNormal.y + inDirection.y);
	return finalVector;
}

double Utils::degreeToRadian(double degree)
{
	return degree * PI / 180;
}

bool Utils::circleCircleCollision(float c1x, float c1y, float c1r, float c2x, float c2y, float c2r)
{
	if (getDistance(c1x, c1y, c2x, c2y) <= c1r + c2r) {
		return true;
	}
	return false;
}

bool Utils::lineLineCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, sf::Vector2f& outIntersectionPoint)
{
	// calculate the distance to intersection point
	float uA = ((x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));
	float uB = ((x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)) / ((y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		outIntersectionPoint.x = x1 + uA * (x2 - x1);
		outIntersectionPoint.y = y1 + uA * (y2 - y1);
		return true;
	}

	return false;
}

float Utils::clamp01(float value) {
    if (value < 0.0)
        return 0.0f;
    return value > 1.0 ? 1 : value;
}

double Utils::lerp(float a, float b, float t) {
    return a - (b - a) * clamp01(t);
}

// LINE/POINT
bool Utils::linePointCollision(float x1, float y1, float x2, float y2, float px, float py)
{
	// get distance from the point to the two ends of the line
	const float d1 = getDistance(px, py, x1, y1);
	const float d2 = getDistance(px, py, x2, y2);

	// get the length of the line
	const float lineLen = getDistance(x1, y1, x2, y2);

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	const float buffer = 20.f;    // higher # = less accurate

	// if the two distances are equal to the line's 
	// length, the point is on the line!
	// note we use the buffer here to give a range, 
	// rather than one #
	return d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer;
}

bool Utils::lineCircleCollision(float x1, float y1, float x2, float y2, float cX, float cY, float cR, sf::Vector2f& outImpactPoint)
{
	//// get length of the line
	const float lengthLine = getDistance(x1, y1, x2, y2);

	// get dot product of the line and circle
	const float dot = ((cX - x1)*(x2 - x1) + (cY - y1)*(y2 - y1)) / static_cast<float>(std::pow(lengthLine, 2));

	// find the closest point on the line
	const double closestX = x1 + (dot * (x2 - x1));
	const double closestY = y1 + (dot * (y2 - y1));

	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	if (!linePointCollision(x1, y1, x2, y2, closestX, closestY)) return false;

	// get distance to the closest point
	const float distance = getDistance(closestX, closestY, cX, cY);

	if (distance <= cR)
	{
		outImpactPoint.x = closestX;
		outImpactPoint.y = closestY;
		return true;
	}

	return false;
}

// POINT/CIRCLE
bool Utils::pointCircleCollision(float px, float py, float cx, float cy, float r)
{
	return getDistance(px, py, cx, cy) <= r;
}
