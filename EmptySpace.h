#pragma once
#include <list>
#include <string>
#include "Simulation.h"

class EmptySpace: public Simulation
{
	void initialize();
	void render();
	void handle_event(sf::Event& event);
	void think(unsigned long fram);
	std::string title() {return "Empty Space";}

	std::list<b2Body*> bodies;
	void filter_bodies();

	b2Vec2 initial_position;
	b2Vec2 initial_velocity;
	float position_noise;
	float velocity_noise;
};