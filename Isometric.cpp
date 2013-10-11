#include "Isometric.h"


REGISTER_SIMULATION(Isometric, "isometric");

#include "helpers.h"
#include "rand.h"

#include <array>
#include <algorithm>

enum IsoShift { none, up, down };

sf::ConvexShape make_isometric(
	const sf::Vector2f& postion,
	const sf::Vector2f& dimensions,
	IsoShift vertical=none,
	IsoShift horizontal=none)
{
	std::array<sf::Vector2f, 4> corners;
	corners.fill(postion);

	corners[1].y += dimensions.y;

	corners[2] += dimensions;
	
	corners[3].x += dimensions.x;

	switch(vertical)
	{
	case up:
		corners[2].y -= dimensions.x;
		corners[3].y -= dimensions.x;
		break;
	case down:
		corners[2].y += dimensions.x;
		corners[3].y += dimensions.x;
		break;
	}
	switch(horizontal)
	{
	case up:
		corners[1].x -= dimensions.y;
		corners[2].x -= dimensions.y;
		break;
	case down:
		corners[1].x += dimensions.y;
		corners[2].x += dimensions.y;
		break;
	} 

	sf::ConvexShape shape(4);
	for(int i = 0; i < 4; ++i)
		shape.setPoint(i, corners[i]);
	return shape;
}

void Isometric::initialize()
{
	maxProgress(100);
	for(int i = 0; i < 100; ++i)
	{
		auto shape = make_isometric(
			sf::Vector2f(
				randcentered(500, 400),
				randcentered(400, 600)),
			sf::Vector2f(10, randuniform(30, 70)),
			IsoShift(randint(1, 2)),
			none);

		shapes.push_back(shape);
		addProgress(1);
	}
	std::sort(shapes.begin(), shapes.end(), [](sf::ConvexShape& lhs, sf::ConvexShape& rhs)
	{
		return lhs.getPoint(0).y < rhs.getPoint(0).y;
	});
}

void Isometric::render()
{
	for(auto it = shapes.begin(); it != shapes.end(); ++it)
		drawShape(*it, sf::Color::Black, sf::Color::White, -1);
}