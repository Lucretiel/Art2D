#include "3DPlanes.h"

#include <cmath>
const float depth = 750;
const float depthRatio = 0.5;

sf::Vector2f transformPointSide(const VanishPlane& plane, const sf::Vector2f& point)
{
	if(point.x == 0)
		return sf::Vector2f(
			plane.intersect,
			point.y);

	float xDist = plane.intersect - plane.vanishingPoint.x;

	float relativeDepth = pow(depthRatio, point.x/depth);

	float x = (relativeDepth * xDist) + plane.vanishingPoint.x;
	float y = (relativeDepth * point.y) + (plane.vanishingPoint.y * (1-relativeDepth));
	return sf::Vector2f(x, y);
}

//As above, but with X and Y reversed
sf::Vector2f transformPointFloor(const VanishPlane& plane, const sf::Vector2f& point)
{
	if(point.y == 0)
		return sf::Vector2f(
			point.x,
			plane.intersect);

	float yDist = plane.intersect - plane.vanishingPoint.y;

	float relativeDepth = pow(depthRatio, point.y/depth);

	float x = (relativeDepth * point.x) + ((1-relativeDepth) * plane.vanishingPoint.x);
	float y = (relativeDepth * yDist) + plane.vanishingPoint.y;
	return sf::Vector2f(x, y);
}


sf::Vector2f transformPointBack(const VanishPlane& plane, const sf::Vector2f& point)
{
	//In this case, float intersect is the distance back from the front plane.
	if(plane.intersect == 0) return point;

	float relativeDepth = pow(depthRatio, plane.intersect/depth);

	float x = (relativeDepth * point.x) + ((1-relativeDepth) * plane.vanishingPoint.x);
	float y = (relativeDepth * point.y) + (plane.vanishingPoint.y * (1-relativeDepth));

	return sf::Vector2f(x, y);
}

sf::Vector2f noTransform(const VanishPlane& plane, const sf::Vector2f& point)
{
	return point;
}