#pragma once
#include <vector>
#include <memory>
#include "simulation.h"

class VanishPlane;

typedef sf::Vector2f (*TransformFunc)(const VanishPlane&, const sf::Vector2f&);

sf::Vector2f transformPointSide(const VanishPlane&, const sf::Vector2f&);
sf::Vector2f transformPointFloor(const VanishPlane&, const sf::Vector2f&);
sf::Vector2f transformPointBack(const VanishPlane&, const sf::Vector2f&);
sf::Vector2f noTransform(const VanishPlane&, const sf::Vector2f&);

class VanishPlane
{
public:
	TransformFunc m_transform;
	sf::Vector2f vanishingPoint;
	float intersect;
	sf::Vector2f translate;

	VanishPlane(
			TransformFunc func=&noTransform,
			const sf::Vector2f& vanish=sf::Vector2f(0, 0),
			float intersect=0,
			const sf::Vector2f& translate = sf::Vector2f(0, 0))
		:m_transform(func)
		,vanishingPoint(vanish)
		,intersect(intersect)
		,translate(translate)
	{}

	sf::Vector2f transformPoint(const sf::Vector2f& point) const
	{
		return m_transform(*this, point + translate);
	}
	sf::Vector2f operator() (const sf::Vector2f& point) const
	{
		return transformPoint(point);
	}

	static VanishPlane sidePlane(
		const sf::Vector2f& vanishingPoint,
		float intersect,
		const sf::Vector2f& translate = sf::Vector2f(0, 0))
	{
		return VanishPlane(&transformPointSide, vanishingPoint, intersect, translate);
	}
	static VanishPlane floorPlane(
		const sf::Vector2f& vanishingPoint,
		float intersect,
		const sf::Vector2f& translate = sf::Vector2f(0, 0))
	{
		return VanishPlane(&transformPointFloor, vanishingPoint, intersect, translate);
	}
	static VanishPlane backPlane(
		const sf::Vector2f& vanishingPoint,
		float intersect,
		const sf::Vector2f& translate = sf::Vector2f(0, 0))
	{
		return VanishPlane(&transformPointBack, vanishingPoint, intersect, translate);
	}
};

template<class Shape, class Transform>
Shape transformShape(const Shape& shape, Transform&& transform)
{
	Shape copy(shape);
	auto vertexCount = copy->m_vertexCount;
	for(int i = 0; i < vertexCount; ++i)
		copy.setPoint(i, transform(copy.getPoint(i)));
	return shape;
}