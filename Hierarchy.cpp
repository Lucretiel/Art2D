#include "Huge.h"
#include "color_helpers.h"
#include <array>
#include <list>

class Hierarchy: public Huge
{
	std::string title() override {return "Hierarchy";}
	std::vector<sf::RectangleShape> rects;
	/*
	InitParams prepare() override
	{
		InitParams params;
		params.videoMode = sf::VideoMode(1000, 1000);
		params.settings.antialiasingLevel = 16;
		return params;
	}*/

	void initialize() override
	{
		const auto size = windowDimensions();
		linear_scale<float, float> window_scale(0, 1000, 0, size.x);
		linear_scale<float, float> lower_bottom(0, size.x, 5 * size.y / 6, 5 * size.y / 6);
		linear_scale<float, float> lower_top(0, size.x, 2 * size.y / 3, size.y / 6);
		linear_scale<float, float> upper_bottom(0, size.x, size.y / 3, 5 * size.y / 6);
		linear_scale<float, float> upper_top(0, size.x, size.y / 6, size.y / 6);
		linear_scale<float, float> x_scale(0, std::sqrt(size.x), 0, size.x);
		linear_color_scale<float> color_scale(0, size.x, sf::Color(128, 128, 128), sf::Color::Black);
		linear_scale<float, float> size_scale(0, size.x, 1, 6);

		const unsigned num_rects(15);
		sf::RectangleShape rect(sf::Vector2f(window_scale(100), window_scale(20)));
		rect.setOrigin(window_scale(50), window_scale(10));
		rect.setFillColor(sf::Color::Black);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(window_scale(-1));

		sf::Vector2f top_left(0, 0);
		sf::Vector2f bottom_right(std::sqrt(size.x), size.y);
		maxProgress(num_rects);
		for(unsigned i = 0; i < num_rects; ++i)
		{
			float x = x_scale(randuniform(0, std::sqrt(size.x)));
			float scale = size_scale(x);
			rect.setScale(scale, scale);
			rect.setOutlineThickness(window_scale(-1)/scale);
			float y = linear_scale<float, float>(0, size.y, upper_top(x), upper_bottom(x))(randuniform(0, size.y));
			rect.setFillColor(color_scale(x));
			rect.setPosition(x, y);
			rects.push_back(rect);
			addProgress(0.5);
			y = linear_scale<float, float>(0, size.y, lower_top(x), lower_bottom(x))(randuniform(0, size.y));
			rect.setFillColor(sf::Color::Black);
			rect.setPosition(x, y);
			rects.push_back(rect);
			addProgress(0.5);
		}
		std::sort(rects.begin(), rects.end(), [](const sf::RectangleShape& lhs, const sf::RectangleShape& rhs)
		{
			return lhs.getPosition().x > rhs.getPosition().x;
		});

	}


	void render() override
	{
		for(const sf::RectangleShape& rect : rects)
			window().draw(rect);
	}

};

REGISTER_SIMULATION(Hierarchy, "hierarchy");
