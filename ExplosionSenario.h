#pragma once

#include "Simulation.h"

class ExplosionSenario: public Simulation
{
private:
	std::vector<b2Body*> boxes;

	void initialize();
	void handle_event(sf::Event& event);
	void render();
	std::string title() {return "EXPLOSION!";}
};