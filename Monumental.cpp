#include "Simulation.h"
#include "3DPlanes.h"
#include "helpers.h"
#include "rand.h"
#include <vector>

class Monumental : public Simulation
{
	struct PlaneGroup
	{
		std::vector<sf::ConvexShape> rects;
	};

	std::vector<PlaneGroup> planes;

	std::string title() override {return "Monumental";}

	template<class Shape>
	sf::ConvexShape convertRectToConvex(const Shape& base, const VanishPlane& plane)
	{
		auto transform = base.getTransform();
		const unsigned n_points = base.getPointCount();
		sf::ConvexShape result(n_points);
		for(unsigned i = 0; i < n_points; ++i)
			result.setPoint(i, plane(transform.transformPoint(base.getPoint(i))));
		result.setFillColor(sf::Color::Black);
		result.setOutlineColor(sf::Color::White);
		result.setOutlineThickness(1);
		return result;
	}

	void initialize() override
	{
		const float max_depth = 6000;
		const float min_depth = 500;
		const unsigned num_planes = 50;
		const unsigned rects_per_plane = 10;
		const float interval = ((max_depth - min_depth) / num_planes);
		const auto center = windowHalfDimensions();
		const auto dimensions = windowDimensions();
		const sf::Vector2f rect_size(50, 250);

		auto plane = VanishPlane::backPlane(sf::Vector2f(center.x, center.y*1.25), 0);
		planes.reserve(num_planes);

		sf::RectangleShape shape_base(rect_size);
		shape_base.setOrigin(rect_size.x / 2, rect_size.y);
		const unsigned num_points = shape_base.getPointCount();
		
		maxProgress(num_planes * rects_per_plane);

		for(unsigned n_plane = num_planes; n_plane > 0; --n_plane)
		{
			plane.intersect = (n_plane * interval) + min_depth;

			planes.emplace_back();
			auto& rects = planes.back().rects;
			rects.reserve(rects_per_plane);

			if(n_plane == int(num_planes*.5))
			{
				sf::Vector2f mighty_size(1000, 12000);
				sf::RectangleShape mighty(mighty_size);
				mighty.setOrigin(mighty_size.x / 2, mighty_size.y);
				mighty.setPosition(center.x, dimensions.y);
				rects.push_back(convertRectToConvex(mighty, plane));
			}


			for(unsigned rect = 0; rect < rects_per_plane; ++rect)
			{
				shape_base.setPosition(randcentered(center.x, center.x * 10), dimensions.y);
				rects.push_back(convertRectToConvex(shape_base, plane));
				addProgress(1);
			}
		}
	}

	void render() override
	{
		for(auto& plane : planes)
			for(auto& rect: plane.rects)
				window().draw(rect);
	}

};

#include "TypeRegistry.h"
REGISTER_SIMULATION(Monumental, "monumental");