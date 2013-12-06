#include "Huge.h"
#include "color_helpers.h"
#include <array>
#include <list>

class Harmony: public Huge
{
	std::string title() override {return "Harmony";}
	std::list<sf::RectangleShape> rects;
	/*
	InitParams prepare() override
	{
		InitParams params;
		params.videoMode = sf::VideoMode(1000, 1000);
		params.settings.antialiasingLevel = 16;
		return params;
	}
	*/
	void initialize() override
	{
		const auto size = windowDimensions();
		linear_scale<float, float> window_scale(0, 1000, 0, size.x);
		linear_scale<float, float> lower_bottom(0, size.x, size.y, 5 * size.y / 6.0);
		linear_scale<float, float> lower_top(0, size.x, 0, 2 * size.y / 3.0);
		linear_scale<float, float> upper_bottom(0, size.x, size.y, size.y / 3.0);
		linear_scale<float, float> upper_top(0, size.x, 0, size.y / 6.0);
		linear_scale<float, float> angle_scale(0, size.x, 45, 0);
		linear_color_scale<float> color_scale(0, size.x, sf::Color::Black, sf::Color(128, 128, 128));

		const unsigned num_rects(300);
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
			rect.setRotation(randcentered(0, angle_scale(pre_location.x)));
			if(randint(0, 2))
			{
				//Top half
				rect.setFillColor(color_scale(pre_location.x));
				pre_location.y = linear_scale<float, float>(0, size.y, upper_top(pre_location.x), upper_bottom(pre_location.x))(pre_location.y);
				rect.setPosition(pre_location);
				rects.push_front(rect);
			}
			else
			{
				rect.setFillColor(sf::Color::Black);
				pre_location.y = linear_scale<float, float>(0, size.y, lower_top(pre_location.x), lower_bottom(pre_location.x))(pre_location.y);
				rect.setPosition(pre_location);
				rects.push_back(rect);
			}
			
			addProgress(1);
		}
	}


	void render() override
	{
		for(const sf::RectangleShape& rect : rects)
			window().draw(rect);
	}

};

REGISTER_SIMULATION(Harmony, "harmony");
