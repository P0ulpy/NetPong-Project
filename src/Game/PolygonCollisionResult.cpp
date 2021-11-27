#include "PolygonCollisionResult.hpp"

#include <iostream>

#include "Entities/PongBall.hpp"

PolygonCollisionResult::PolygonCollisionResult()
{
	
}

PolygonCollisionResult::~PolygonCollisionResult()
{

}

// Calculate the projection of a polygon on an axis
// and returns it as a [min, max] interval
void PolygonCollisionResult::ProjectPolygon(sf::Vector2f axis, sf::ConvexShape polygon, float& min, float& max) {

	// To project a point on an axis use the dot product
	float dotProduct = PongBall::dot(axis, polygon.getPoint(0));

	min = dotProduct;
	max = dotProduct;

	for (int i = 0; i < polygon.getPointCount(); i++)
	{
		dotProduct = PongBall::dot(polygon.getPoint(i), axis);

		if (dotProduct < min)
		{
			min = dotProduct;
		}
		else if (dotProduct > max)
		{
			max = dotProduct;
		}
	}
}

// Calculate the projection of a polygon on an axis
// and returns it as a [min, max] interval
void PolygonCollisionResult::ProjectPolygon(sf::Vector2f axis, sf::RectangleShape rectPolygon, float& min, float& max) {

	// To project a point on an axis use the dot product
	float dotProduct = PongBall::dot(axis, rectPolygon.getPoint(0));

	min = dotProduct;
	max = dotProduct;

	for (int i = 0; i < rectPolygon.getPointCount(); i++)
	{
		dotProduct = PongBall::dot(rectPolygon.getPoint(i), axis);

		if (dotProduct < min)
		{
			min = dotProduct;
		}
		else if (dotProduct > max)
		{
			max = dotProduct;
		}
	}
}

// Calculate the projection of a polygon on an axis
// and returns it as a [min, max] interval
void PolygonCollisionResult::ProjectPolygon(sf::Vector2f axis, sf::CircleShape ballPolygon, float& min, float& max) {

	// To project a point on an axis use the dot product
	float dotProduct = PongBall::dot(axis, ballPolygon.getPoint(0));

	min = dotProduct;
	max = dotProduct;

	for (int i = 0; i < ballPolygon.getPointCount(); i++)
	{
		dotProduct = PongBall::dot(ballPolygon.getPoint(i), axis);

		if (dotProduct < min)
		{
			min = dotProduct;
		}
		else if (dotProduct > max)
		{
			max = dotProduct;
		}
	}
}

// Calculate the distance between [minA, maxA] and [minB, maxB]
// The distance will be negative if the intervals overlap
float PolygonCollisionResult::IntervalDistance(float minA, float maxA, float minB, float maxB) {
	if (minA < minB)
	{
		return minB - maxA;
	}
	else
	{
		return minA - maxB;
	}
}

// Check if polygon A is going to collide with polygon B.
// The last parameter is the *relative* velocity 
// of the polygons (i.e. velocityA - velocityB)
PolygonCollisionResult PolygonCollisionResult::PolygonCollision(sf::CircleShape polygonBall, sf::RectangleShape polygonRect, sf::Vector2f velocity) {

	PolygonCollisionResult* result = new PolygonCollisionResult();

	result->Intersect = true;
	result->WillIntersect = true;

	int edgeCountA = polygonBall.getPointCount();
	int edgeCountB = polygonRect.getPointCount();

	float minIntervalDistance = INFINITY;
	sf::Vector2f translationAxis;
	sf::Vector2f edge;

	// Loop through all the edges of both polygons
	for (int edgeIndex = 0; edgeIndex < edgeCountA + edgeCountB; edgeIndex++)
	{
		float xA, xB, yA, yB;
		if (edgeIndex < edgeCountA)
		{
			if(edgeIndex+1 != edgeCountA)
			{
				xA = polygonBall.getTransform().transformPoint(sf::Vector2f(polygonBall.getPoint(edgeIndex).x, polygonBall.getPoint(edgeIndex).y)).x;
				yA = polygonBall.getTransform().transformPoint(sf::Vector2f(polygonBall.getPoint(edgeIndex).x, polygonBall.getPoint(edgeIndex).y)).y;
				xB = polygonBall.getTransform().transformPoint(sf::Vector2f(polygonBall.getPoint(edgeIndex + 1).x, polygonBall.getPoint(edgeIndex + 1).y)).x;
				yB = polygonBall.getTransform().transformPoint(sf::Vector2f(polygonBall.getPoint(edgeIndex + 1).x, polygonBall.getPoint(edgeIndex + 1).y)).y;
			}
			else
			{
				xA = polygonBall.getTransform().transformPoint(sf::Vector2f(polygonBall.getPoint(edgeIndex).x, polygonBall.getPoint(edgeIndex).y)).x;
				yA = polygonBall.getTransform().transformPoint(sf::Vector2f(polygonBall.getPoint(edgeIndex).x, polygonBall.getPoint(edgeIndex).y)).y;
				xB = polygonBall.getTransform().transformPoint(sf::Vector2f(polygonBall.getPoint(0).x, polygonBall.getPoint(0).y)).x;
				yB = polygonBall.getTransform().transformPoint(sf::Vector2f(polygonBall.getPoint(0).x, polygonBall.getPoint(0).y)).y;
			}


			edge = sf::Vector2f(xB - xA, yB - yA);

			//if (edgeIndex == 0) std::cout << "Edge : " << edge.x << " ; " << edge.y << " | xA = " << xA << " ; yA = " << yA << " ; xB = " << xB << " ; yB = " << yB << std::endl;
		}
		else
		{
			if(edgeIndex+1 != edgeCountA + edgeCountB)
			{
				xA = polygonRect.getTransform().transformPoint(sf::Vector2f(polygonRect.getPoint(edgeIndex - edgeCountA).x, polygonRect.getPoint(edgeIndex - edgeCountA).y)).x;
				yA = polygonRect.getTransform().transformPoint(sf::Vector2f(polygonRect.getPoint(edgeIndex - edgeCountA).x, polygonRect.getPoint(edgeIndex - edgeCountA).y)).y;
				xB = polygonRect.getTransform().transformPoint(sf::Vector2f(polygonRect.getPoint(edgeIndex - edgeCountA + 1).x, polygonRect.getPoint(edgeIndex - edgeCountA + 1).y)).x;
				yB = polygonRect.getTransform().transformPoint(sf::Vector2f(polygonRect.getPoint(edgeIndex - edgeCountA + 1).x, polygonRect.getPoint(edgeIndex - edgeCountA + 1).y)).y;
			}
			else
			{
				xA = polygonRect.getTransform().transformPoint(sf::Vector2f(polygonRect.getPoint(edgeIndex - edgeCountA).x, polygonRect.getPoint(edgeIndex - edgeCountA).y)).x;
				yA = polygonRect.getTransform().transformPoint(sf::Vector2f(polygonRect.getPoint(edgeIndex - edgeCountA).x, polygonRect.getPoint(edgeIndex - edgeCountA).y)).y;
				xB = polygonRect.getTransform().transformPoint(sf::Vector2f(polygonRect.getPoint(edgeCountA + 1).x, polygonRect.getPoint(edgeCountA + 1).y)).x;
				yB = polygonRect.getTransform().transformPoint(sf::Vector2f(polygonRect.getPoint(edgeCountA + 1).x, polygonRect.getPoint(edgeCountA + 1).y)).y;
			}

			edge = sf::Vector2f(xB - xA, yB - yA);
		}

		// ===== 1. Find if the polygons are currently intersecting =====

		// Find the axis perpendicular to the current edge
		sf::Vector2f axis = sf::Vector2f(-edge.y, edge.x);
		axis = PongBall::normalize(axis);

		// Find the projection of the polygon on the current axis
		float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
		ProjectPolygon(axis, polygonBall, minA, maxA);
		ProjectPolygon(axis, polygonRect, minB, maxB);

		// Check if the polygon projections are currentlty intersecting
		float intervDist = IntervalDistance(minA, maxA, minB, maxB);

		if (edgeIndex == 0) std::cout << "IntervalDist : " << intervDist << " | minA = " << minA << " ; maxA = " << maxA << " ; minB = " << minB << " ; maxB = " << maxB << std::endl;

		if (intervDist > 0)
		{
			result->Intersect = false;
			std::cout << "False" << std::endl;
		}
		else
		{

		}


		// ===== 2. Now find if the polygons *will* intersect =====

		// Project the velocity on the current axis
		float velocityProjection = PongBall::dot(axis, velocity);

		// Get the projection of polygon A during the movement
		if (velocityProjection < 0)
		{
			minA += velocityProjection;
		}
		else
		{
			maxA += velocityProjection;
		}

		// Do the same test as above for the new projection
		float intervalDistance = IntervalDistance(minA, maxA, minB, maxB);
		if (intervalDistance > 0)
		{
			result->WillIntersect = false;
			std::cout << "False" << std::endl;

		}
		// If the polygons are not intersecting and won't intersect, exit the loop
		if (!result->Intersect && !result->WillIntersect) break;

		// Check if the current interval distance is the minimum one. If so store
		// the interval distance and the current distance.
		// This will be used to calculate the minimum translation vector
		intervalDistance = std::abs(intervalDistance);
		if (intervalDistance < minIntervalDistance)
		{
			minIntervalDistance = intervalDistance;
			translationAxis = axis;

			sf::Vector2f polygonAcenter = sf::Vector2f((polygonBall.getGlobalBounds().left + polygonBall.getGlobalBounds().width) / 2,
				(polygonBall.getGlobalBounds().top + polygonBall.getGlobalBounds().height) / 2);

			sf::Vector2f polygonBcenter = sf::Vector2f((polygonRect.getGlobalBounds().left + polygonRect.getGlobalBounds().width) / 2,
				(polygonRect.getGlobalBounds().top + polygonRect.getGlobalBounds().height) / 2);

			sf::Vector2f currentDistance = polygonAcenter - polygonBcenter;
			if (PongBall::dot(currentDistance, translationAxis) < 0)
			{
				translationAxis = -translationAxis;
			}
		}
	}

	// The minimum translation vector
	// can be used to push the polygons appart.
	if (result->WillIntersect)
	{
		result->MinimumTranslationVector = translationAxis * minIntervalDistance;
	}

	return *result;
}