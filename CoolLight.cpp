#include "ShadowSystem.h"

class CoolLight : public ShadowSystem
{
	std::string title() override {return "Cool Light";}

	void initialize() override
	{
		set_layers(sf::Vector2f(5,-20), sf::Vector2f(30,-120), sf::Vector2f(5, -20));
		get_layer(0).shadow_color = sf::Color(100, 100, 0);
		get_layer(1).shadow_color = sf::Color(100, 100, 0);
		get_layer(0).background = sf::Color(100, 100, 128);
		//get_layer(0).background = sf::Color::White;
	}

	void layer_render() override
	{
		sf::RectangleShape shape(sf::Vector2f(100, 500));
		shape.setPosition(100, 200);
		shape.setFillColor(sf::Color(50, 0, 50));
		draw_shadowed_object(shape, 1);
		shape.setPosition(220, 200);
		shape.setFillColor(sf::Color(50, 50, 50));
		draw_shadowed_object(shape, 1);
		shape.setPosition(340, 200);
		shape.setFillColor(sf::Color(0, 100, 100));
		draw_shadowed_object(shape, 1);
		shape.setPosition(460, 200);
		shape.setFillColor(sf::Color(0, 0, 128));
		draw_shadowed_object(shape, 1);

		sf::RectangleShape hang1(sf::Vector2f(500, 100));
		hang1.setPosition(150, 575);
		hang1.setFillColor(sf::Color(30, 60, 150));
		draw_shadowed_object(hang1, 2);
	}
};

REGISTER_SIMULATION(CoolLight, "cool-light");