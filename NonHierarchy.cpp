#include "Huge.h"
#include "color_helpers.h"
#include <array>
#include <list>

class NonHierarchy: public Huge
{
	std::string title() override {return "NonHierarchy";}
	std::list<sf::RectangleShape> rects;
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
		linear_scale<float, float> upper_scale(0, size.y, size.y / 6.0, size.y / 3.0);
		linear_scale<float, float> lower_scale(0, size.y, 2 * size.y / 3.0, 5 * size.y / 6.0);

		const unsigned num_rects(100);
		sf::RectangleShape rect(sf::Vector2f(window_scale(100), window_scale(20)));
		rect.setFillColor(sf::Color::Black);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(window_scale(-1));

		sf::Vector2f top_left(window_scale(-100), 0);
		sf::Vector2f bottom_right(size.x + window_scale(100), size.y);
		maxProgress(num_rects);
		for(unsigned i = 0; i < num_rects; ++i)
		{
			auto pre_location = random_in(top_left, bottom_right);
			if(randint(0, 2))
			{
				//Top half
				rect.setFillColor(sf::Color(128, 128, 128));
				pre_location.y = upper_scale(pre_location.y);
			}
			else
			{
				rect.setFillColor(sf::Color::Black);
				pre_location.y = lower_scale(pre_location.y);
			}
			
			rect.setPosition(pre_location);
			rects.push_back(rect);
			addProgress(1);
		}
	}


	void render() override
	{
		for(const sf::RectangleShape& rect : rects)
			window().draw(rect);
	}

};

REGISTER_SIMULATION(NonHierarchy, "non-hierarchy");
