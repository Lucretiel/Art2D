#pragma once
#include <array>
#include <cmath>
#include <Box2D\Box2D.h>
#include <SFML/Graphics.hpp>

template<class Shape, unsigned num_extra_fixtures=0>
struct shapeDef
{
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	Shape shape;
	struct shape_fixture
	{
		b2FixtureDef fixtureDef;
		Shape shape;
		shape_fixture()
		{
			fixtureDef.shape = &shape;
		}

	};
	std::array<shape_fixture, num_extra_fixtures> extra_fixtures;
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
	for(const auto& fixt : shape.extra_fixtures)
		body->CreateFixture(&fixt.fixtureDef);
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
T random_centered(const T& center, const T& extents)
{
	return T(
		randcentered(center.x, extents.x),
		randcentered(center.y, extents.y));
}

template<class T>
T random_centered(const T& center, float range)
{
	return random_centered(center, T(range, range));
}


template<class T>
T random_in(const T& corner1, const T& corner2)
{
	return T(
		randuniform(std::min(corner1.x, corner2.x), std::max(corner1.x, corner2.x)),
		randuniform(std::min(corner1.y, corner2.y), std::max(corner1.y, corner2.y)));
}


//VECTORS
inline float length(sf::Vector2f vec)
{
	return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

inline sf::Vector2f rot90(sf::Vector2f vec)
{
	return sf::Vector2f(-vec.y, vec.x);
}

inline sf::Vector2f to_length(sf::Vector2f vec, float len)
{
	float factor = len / length(vec);
	return vec * factor;
}

template<class T>
struct range
{
	T min;
	T max;

	range(T min=0, T max=0):
		min(min),
		max(max)
	{}

	range(const range& cpy):
		min(cpy.min),
		max(cpy.max)
	{}

	T diff() const { return max - min; };

	T clamp(T input)
	{
		return std::min(std::max(input, min), max);
	}

};

template<typename Input, typename Output>
struct linear_scale
{
	range<Input> input_range;
	range<Output> output_range;

	linear_scale(
		Input input_min,
		Input input_max,
		Output output_min,
		Output output_max)

		:input_range(input_min, input_max)
		,output_range(output_min, output_max)
	{}

	linear_scale(range<Input> input_range, range<Output> output_range)
		:input_range(input_range)
		,output_range(output_range)
	{}

	linear_scale()
	{}

	Output operator()(Input input) const
	{
		return output_range.min == output_range.max
			? output_range.min
			: static_cast<Output>((((input - input_range.min) * output_range.diff()) / input_range.diff()) + output_range.min);
	}

};