#include "Simulation.h"
#include "3DPlanes.h"
#include "helpers.h"
#include "rand.h"
#include <vector>

class Intimate : public Simulation
{
	struct PlaneGroup
	{
		std::vector<sf::ConvexShape> rects;
	};

	std::vector<PlaneGroup> planes;

	std::string title() override {return "Intimate";}

	template<class Shape>
	sf::ConvexShape convertRectToConvex(const Shape& base, const VanishPlane& plane)
	{
		auto transform = base.getTransform();
		const unsigned n_points = base.getPointCount();
		sf::ConvexShape result(n_points);
		for(unsigned i = 0; i < n_points; ++i)
			result.setPoint(i, plane(transform.transformPoint(base.getPoint(i))));
		result.setFillColor(base.getFillColor());
		result.setOutlineColor(base.getOutlineColor());
		result.setOutlineThickness(1);
		return result;
	}

	void initialize() override
	{
		const float max_depth = 6000;
		const float min_depth = -300;
		const unsigned num_planes = 75;
		const unsigned rects_per_plane = 10;
		const float interval = ((max_depth - min_depth) / num_planes);
		const auto center = windowHalfDimensions();
		const auto dimensions = windowDimensions();

		auto plane = VanishPlane::backPlane(sf::Vector2f(center.x, center.y*1.75), 0);
		planes.reserve(num_planes);

		const sf::Vector2f rect_size(50, 250);
		sf::RectangleShape shape_base(rect_size);
		shape_base.setOrigin(rect_size.x / 2, rect_size.y);
		shape_base.setFillColor(sf::Color::Black);
		shape_base.setOutlineColor(sf::Color::White);
		const unsigned num_points = shape_base.getPointCount();

		const sf::Vector2f ground_size(100000, 100);
		sf::RectangleShape ground(ground_size);
		ground.setOrigin(ground_size.x / 2, 0);
		ground.setPosition(center.x, dimensions.y);
		ground.setFillColor(sf::Color::White);
		ground.setOutlineColor(sf::Color::Transparent);
		
		maxProgress(num_planes * (rects_per_plane+1));

		for(unsigned n_plane = num_planes; n_plane > 0; --n_plane)
		{
			plane.intersect = (n_plane * interval) + min_depth;

			planes.emplace_back();
			auto& rects = planes.back().rects;
			rects.reserve(rects_per_plane);

			if(n_plane == int(num_planes*.8))
			{
				const sf::Vector2f mighty_size(2000, 200000);
				sf::RectangleShape mighty(mighty_size);
				mighty.setOrigin(mighty_size.x / 2, mighty_size.y);
				mighty.setPosition(center.x * 30, dimensions.y);
				mighty.setFillColor(sf::Color::Black);
				mighty.setOutlineColor(sf::Color::White);
				rects.push_back(convertRectToConvex(mighty, plane));
			}


			for(unsigned rect = 0; rect < rects_per_plane; ++rect)
			{
				shape_base.setPosition(
					randcentered(center.x, center.x * sqrt(n_plane) * 5),
					dimensions.y + randuniform(0, 50));
				shape_base.setRotation(randuniform(-30, 30));
				rects.push_back(convertRectToConvex(shape_base, plane));
				addProgress(1);
			}
			rects.push_back(convertRectToConvex(ground, plane));
			addProgress(1);
		}
	}

	void render() override
	{
		for(auto& plane : planes)
			for(auto& rect: plane.rects)
				window().draw(rect);
	}

};


REGISTER_SIMULATION(Intimate, "intimate");