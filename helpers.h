#pragma once
#include <Box2D\Box2D.h>

template<class Shape>
struct shapeDef
{
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	Shape shape;

	shapeDef()
	{
		fixtureDef.shape = &shape;
	}
};

typedef shapeDef<b2PolygonShape> polygonDef;
typedef shapeDef<b2CircleShape> circleDef;

b2Body* make_box(b2World& world, const b2Vec2& center, const b2Vec2& extents, float angle=0, short group=0);

template<class Shape>
b2Body* make_shape(b2World& world, const shapeDef<Shape>& shape)
{
	auto body = world.CreateBody(&shape.bodyDef);
	body->CreateFixture(&shape.fixtureDef);
	return body;
}

double to_radians(double degrees);
double to_degrees(double radians);

template<class T>
T random_margin(const T& dimensions, float margin)
{
	return T(
		randuniform(margin, dimensions.x - margin),
		randuniform(margin, dimensions.y - margin));
}

template<class T>
T random_centered(const T& center, float range)
{
	return T(
		center.x + randuniform(-range, range),
		center.y + randuniform(-range, range));
}