#include "ShadowSystem.h"

class LocalColor : public ShadowSystem
{
	std::string title() override {return "Local Color";}

	void initialize() override
	{
		set_layers(sf::Vector2f(5,-5), sf::Vector2f(30,-30), sf::Vector2f(5, -5));
		get_layer(0).shadow_color = sf::Color(128, 128, 128);
		get_layer(1).shadow_color = sf::Color(128, 128, 218);
		get_layer(0).background = sf::Color(200, 200, 255);
		//get_layer(0).background = sf::Color::White;
	}

	void layer_render() override
	{
		sf::RectangleShape shape(sf::Vector2f(100, 500));
		shape.setPosition(100, 200);
		shape.setFillColor(sf::Color::Red);
		draw_shadowed_object(shape, 1);
		shape.setPosition(220, 200);
		shape.setFillColor(sf::Color::Yellow);
		draw_shadowed_object(shape, 1);
		shape.setPosition(340, 200);
		shape.setFillColor(sf::Color::Green);
		draw_shadowed_object(shape, 1);
		shape.setPosition(460, 200);
		shape.setFillColor(sf::Color(0, 0, 150));
		draw_shadowed_object(shape, 1);

		sf::RectangleShape hang1(sf::Vector2f(500, 100));
		hang1.setPosition(150, 575);
		hang1.setFillColor(sf::Color::White);
		draw_shadowed_object(hang1, 2);
	}
};

REGISTER_SIMULATION(LocalColor, "local");