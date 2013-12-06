#include "ShadowSystem.h"

class WarmLight : public ShadowSystem
{
	std::string title() override {return "Warm Light";}

	void initialize() override
	{
		set_layers(sf::Vector2f(15,-5), sf::Vector2f(90,-30), sf::Vector2f(15, -5));
		get_layer(0).shadow_color = sf::Color(100, 100, 0);
		get_layer(1).shadow_color = sf::Color(100, 100, 0);
		get_layer(0).background = sf::Color(200, 100, 50);
		//get_layer(0).background = sf::Color::White;
	}

	void layer_render() override
	{
		sf::RectangleShape shape(sf::Vector2f(100, 500));
		shape.setPosition(100, 200);
		shape.setFillColor(sf::Color(200, 50, 0));
		draw_shadowed_object(shape, 1);
		shape.setPosition(220, 200);
		shape.setFillColor(sf::Color(255, 100, 0));
		draw_shadowed_object(shape, 1);
		shape.setPosition(340, 200);
		shape.setFillColor(sf::Color(20, 30, 0));
		draw_shadowed_object(shape, 1);
		shape.setPosition(460, 200);
		shape.setFillColor(sf::Color(5, 5, 20));
		draw_shadowed_object(shape, 1);

		sf::RectangleShape hang1(sf::Vector2f(500, 100));
		hang1.setPosition(150, 575);
		hang1.setFillColor(sf::Color(255, 160, 50));
		draw_shadowed_object(hang1, 2);
	}
};

REGISTER_SIMULATION(WarmLight, "warm-light");